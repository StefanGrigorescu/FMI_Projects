package com.eastwoodjtb.services.impl;

import antlr.MismatchedTokenException;
import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.eastwoodjtb.models.dto.*;
import com.eastwoodjtb.models.entities.Role;
import com.eastwoodjtb.models.entities.SoundEquipUser;
import com.eastwoodjtb.repositories.RoleRepository;
import com.eastwoodjtb.repositories.SoundEquipUserRepository;
import com.eastwoodjtb.security.AuthUtil;
import com.eastwoodjtb.security.RoleEnum;
import com.eastwoodjtb.security.TokenUtil;
import com.eastwoodjtb.services.SoundEquipUserService;
import com.eastwoodjtb.utils.ListAccesser;
import com.eastwoodjtb.utils.LogMarkers;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.transaction.Transactional;
import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

import static java.text.MessageFormat.format;
import static org.springframework.beans.BeanUtils.copyProperties;
import static org.springframework.http.HttpHeaders.AUTHORIZATION;


@Service
@RequiredArgsConstructor
@Transactional
@Slf4j
public class SoundEquipUserServiceImpl implements SoundEquipUserService, UserDetailsService {
    private final SoundEquipUserRepository soundEquipUserRepository;
    private final RoleRepository roleRepository;
    private final PasswordEncoder passwordEncoder;


    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        SoundEquipUser soundEquipUser;
        try {
            soundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.loadUserByUsername: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    username));
            throw new UsernameNotFoundException("SoundEquipUserServiceImpl.loadUserByUsername: Two users with the same username (username = {0}) were found in the database! Prompt action is required!.");
        }
        if(soundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.loadUserByUsername: User with username = {} not found in the database.",
                    username);
            throw new UsernameNotFoundException("SoundEquipUserServiceImpl.loadUserByUsername: User not found in the database.");
        }
        log.info("SoundEquipUserServiceImpl.loadUserByUsername: User with username = {} found in the database.",
                username);
        Collection<SimpleGrantedAuthority> authorities = new ArrayList<>();
        soundEquipUser.getRoles().forEach(role ->
                authorities.add(new SimpleGrantedAuthority(role.getName())));
        return new org.springframework.security.core.userdetails.User(username, soundEquipUser.getPassword(), authorities);
    }

    @Override
    public boolean register(RegisterDTO registerDTO) {
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(registerDTO.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.register: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    registerDTO.getName()));
            return false;
        }
        if (existingSoundEquipUser != null) {
            log.error("SoundEquipUserServiceImpl.register: Attempting to insert an already existing user! Try a different username.");
            return false;
        }
        SoundEquipUser newUser = toEntity(registerDTO);
        registerDTO.setPassword(passwordEncoder.encode(registerDTO.getPassword()));
        soundEquipUserRepository.save(newUser);
        log.info("SoundEquipUserServiceImpl.register: Saved new user with username = {}.", newUser.getName());
        return true;
    }

    @Override
    public void refreshTokens(HttpServletRequest request, HttpServletResponse response)  throws IOException
    {
        String authorizationHeader = request.getHeader(AUTHORIZATION);
        if(authorizationHeader == null || !authorizationHeader.startsWith("Bearer "))
            throw new RuntimeException("SoundEquipUserServiceImpl.refreshToken: Refresh token is missing.");

        try {
            DecodedJWT decodedJWT = TokenUtil.getDecodedJWT(authorizationHeader);

            String username = decodedJWT.getSubject();
            checkRefreshToken(username, authorizationHeader.substring("Bearer ".length()));

            SoundEquipUser soundEquipUser;
            try {
                soundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
            }
            catch (DataIntegrityViolationException e)
            {
                log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.refreshToken: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                        username));
                return;
            }
            if(soundEquipUser == null)
                throw new NullPointerException("SoundEquipUserServiceImpl.refreshToken: Couldn't retrieve the specified user. Check in previous logs for more details.");

            Algorithm algorithm = AuthUtil.getAlgorithm();

            String accessToken = JWT.create()
                    .withSubject(soundEquipUser.getName())
                    .withExpiresAt(new Date(System.currentTimeMillis() + AuthUtil.getAccessTokenLifespan()))
                    .withIssuer(request.getRequestURL().toString())
                    .withClaim(AuthUtil.getRolesClaimName(), soundEquipUser.getRoles().stream().map(Role::getName).collect(Collectors.toList()))
                    .sign(algorithm);

            String refreshToken = JWT.create()
                    .withSubject(soundEquipUser.getName())
                    .withExpiresAt(new Date(System.currentTimeMillis() + AuthUtil.getRefreshTokenLifespan()))
                    .withIssuer(request.getRequestURL().toString())
                    .withClaim(AuthUtil.getRolesClaimName(), soundEquipUser.getRoles().stream().map(Role::getName).collect(Collectors.toList()))
                    .sign(algorithm);

            TokenUtil.setResponseTokens(response, accessToken, refreshToken);
            updateTokens(username, accessToken, refreshToken);
        }
        catch (NullPointerException e)
        {
            log.error(e.getMessage());
        }
        catch (Exception e)
        {
            log.error("SoundEquipUserServiceImpl.refreshToken:: Error logging in: {}", e.getMessage());
            TokenUtil.setResponseError(response, e.getMessage());
        }
    }

    @Override
    public boolean create(SoundEquipUser soundEquipUser) {
        SoundEquipUser existingSoundEquipUser;
        try {
             existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(soundEquipUser.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.create: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    soundEquipUser.getName()));
            return false;
        }
        if (existingSoundEquipUser != null) {
            log.error("SoundEquipUserServiceImpl.create: Attempting to insert an already existing user! Try a different username.");
            return false;
        }
        soundEquipUser.setPassword(passwordEncoder.encode(soundEquipUser.getPassword()));
        soundEquipUserRepository.save(soundEquipUser);
        log.info("SoundEquipUserServiceImpl.create: Saved new user with username = {}.", soundEquipUser.getName());
        return true;
    }

    @Override
    public List<SoundEquipUserDTO> getAll() {
        return soundEquipUserRepository
                .findAll()
                .stream()
                .map(this::toDTO)
                .collect(Collectors.toList());
    }

    @Override
    public SoundEquipUserDTO getOneById(Long id) {
        return toDTO(soundEquipUserRepository.getById(id));
    }

    @Override
    public SoundEquipUserDTO getOneByName(String name) {
        try {
            return toDTO(ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(name)));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.getOneByName: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    name));
            return null;
        }
    }

    @Override
    public boolean update(SoundEquipUser soundEquipUser) {
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(soundEquipUser.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.update: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    soundEquipUser.getName()));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.update: Attempting to update a non existing user!");
            return false;
        }
        soundEquipUserRepository.save(soundEquipUser);
        log.info("SoundEquipUserServiceImpl.update: Updated user with username = {}.", soundEquipUser.getName());
        return true;
    }

    @Override
    public boolean updateFtpState(FtpStateDTO ftpStateDTO) {
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(ftpStateDTO.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.updateFtpState: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    ftpStateDTO.getName()));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.updateFtpState: Attempting to update a non existing user!");
            return false;
        }
        boolean ftpLogoutState = ftpStateDTO.getFtpLogoutState();
        existingSoundEquipUser.setFtpLogoutState(ftpLogoutState);
        if(ftpLogoutState)
        {
            existingSoundEquipUser.setFtpUsername(null);
            existingSoundEquipUser.setFtpPassword(null);
            log.info("SoundEquipUserServiceImpl.updateFtpState: Ftp credentials removed from database for user with username = {}. Relogin will be required at next application run.",
                    existingSoundEquipUser.getName());
        }
        soundEquipUserRepository.save(existingSoundEquipUser);
        log.info("SoundEquipUserServiceImpl.updateFtpState: Updated user with username = {}.", existingSoundEquipUser.getName());
        return true;
    }

    @Override
    public boolean updateFtpCredentials(FtpCredentialsDTO ftpCredentialsDTO) {
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(ftpCredentialsDTO.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.updateFtpCredentials: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    ftpCredentialsDTO.getName()));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.updateFtpCredentials: Attempting to update a non existing user!");
            return false;
        }
        existingSoundEquipUser.setFtpUsername(ftpCredentialsDTO.getFtpUsername());
        existingSoundEquipUser.setFtpPassword(ftpCredentialsDTO.getFtpPassword());
        soundEquipUserRepository.save(existingSoundEquipUser);
        log.info("SoundEquipUserServiceImpl.updateFtpCredentials: Updated user with username = {}.", existingSoundEquipUser.getName());
        return true;
    }

    @Override
    public boolean updateTokens(String username, String accessToken, String refreshToken)
    {
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.updateTokens: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    username));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.updateTokens: Attempting to update a non existing user!");
            return false;
        }
        existingSoundEquipUser.setAccessToken(accessToken);
        existingSoundEquipUser.setRefreshToken(refreshToken);
        log.info("SoundEquipUserServiceImpl.updateTokens: Updated tokens for the user with username = {}.", existingSoundEquipUser.getName());
        return true;
    }

    @Override
    public boolean checkAccessToken(String username, String accessToken) throws MismatchedTokenException
    {
        if(accessToken == null)
        {
            log.error("SoundEquipUserServiceImpl.checkAccessToken: The provided access token is null.");
            throw new MismatchedTokenException();
        }
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.checkAccessToken: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    username));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.checkAccessToken: Attempting to check a non existing user!");
            return false;
        }
        if(!Objects.equals(accessToken, existingSoundEquipUser.getAccessToken()))
        {
            log.error("SoundEquipUserServiceImpl.checkAccessToken: The provided access token appears to have been temepered with.");
            throw new MismatchedTokenException();
        }
        log.info("SoundEquipUserServiceImpl.checkAccessToken: Validated access token for the user with username = {}.", existingSoundEquipUser.getName());
        return true;
    }

    @Override
    public boolean checkRefreshToken(String username, String refreshToken) throws MismatchedTokenException
    {
        if(refreshToken == null)
        {
            log.error("SoundEquipUserServiceImpl.checkRefreshToken: The provided refresh token is null.");
            throw new MismatchedTokenException();
        }
        SoundEquipUser existingSoundEquipUser;
        try {
            existingSoundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.checkRefreshToken: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    username));
            return false;
        }
        if(existingSoundEquipUser == null)
        {
            log.error("SoundEquipUserServiceImpl.checkRefreshToken: Attempting to check a non existing user!");
            return false;
        }
        if(!Objects.equals(refreshToken, existingSoundEquipUser.getRefreshToken()))
        {
            log.error("SoundEquipUserServiceImpl.checkRefreshToken: The provided refresh token appears to have been temepered with.");
            throw new MismatchedTokenException();
        }
        log.info("SoundEquipUserServiceImpl.checkRefreshToken: Validated refresh token for the user with username = {}.", existingSoundEquipUser.getName());
        return true;
    }

    @Override
    public boolean addRole(String username, String roleName) {
        SoundEquipUser soundEquipUser;
        try {
            soundEquipUser = ListAccesser.getOnlyOrNull(soundEquipUserRepository.findByName(username));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.addRole: Two users with the same username (username = {0}) were found in the database! Prompt action is required!",
                    username));
            return false;
        }
        if(soundEquipUser == null)
        {
            log.warn(format("SoundEquipUserServiceImpl.addRole: The passed username (username = {0}) does not correspond to any existing user."),
                    username);
            return false;
        }
        Role role;
        try {
            role = ListAccesser.getOnlyOrNull(roleRepository.findByName(roleName));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.addRole: Two roles with the same role name (role name = {0}) were found in the database! Prompt action is required!",
                    roleName));
            return false;
        }
        if(role == null)
        {
            log.warn(format("SoundEquipUserServiceImpl.addRole: The passed role name (role name = {0}) does not correspond to any existing role."),
                    roleName);
            return false;
        }
        soundEquipUser.getRoles().add(role);
        log.info("SoundEquipUserServiceImpl.addRole: Added the role with role name = {} to user with username = {}.",
                roleName,
                username);
        return true;
    }

    @Override
    public boolean delete(Long id) {
        if(!soundEquipUserRepository.existsById(id))
        {
            log.warn("SoundEquipUserServiceImpl.delete: No user with id = {} was found in the database.", id);
            return false;
        }
        soundEquipUserRepository.deleteById(id);
        log.info("SoundEquipUserServiceImpl.delete: Deleted the user with id = {}.", id);
        return true;
    }

    private SoundEquipUserDTO toDTO(SoundEquipUser entity)
    {
        if(entity == null)
            return null;

        SoundEquipUserDTO dto = new SoundEquipUserDTO();
        copyProperties(entity, dto, "password", "ftpPassword", "accessToken", "refreshToken");
        return dto;
    }

    private SoundEquipUser toEntity(RegisterDTO dto)
    {
        if(dto == null)
            return null;
        String roleName = RoleEnum.BasicUser.getName();
        Role role;
        try {
            role = ListAccesser.getOnlyOrNull(roleRepository.findByName(roleName));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("SoundEquipUserServiceImpl.toEntity: Two roles with the same role name (role name = {0}) were found in the database! Prompt action is required!",
                    roleName));
            return null;
        }
        if(role == null) {
            log.warn(format("SoundEquipUserServiceImpl.toEntity: The passed role name (role name = {0}) does not correspond to any existing role."),
                    roleName);
            return null;
        }
        SoundEquipUser entity = new SoundEquipUser();
        copyProperties(dto, entity);
        entity.setAccessToken(null);
        entity.setRefreshToken(null);
        entity.getRoles().add(role);
        log.info("SoundEquipUserServiceImpl.toEntity: Added the role with role name = {} to user with username = {}.",
                roleName,
                entity.getName());
        return entity;
    }

}
