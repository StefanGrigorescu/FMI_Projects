package com.eastwoodjtb.services;

import antlr.MismatchedTokenException;
import com.eastwoodjtb.models.dto.FtpCredentialsDTO;
import com.eastwoodjtb.models.dto.FtpStateDTO;
import com.eastwoodjtb.models.dto.RegisterDTO;
import com.eastwoodjtb.models.dto.SoundEquipUserDTO;
import com.eastwoodjtb.models.entities.SoundEquipUser;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;


public interface SoundEquipUserService {

    boolean register(RegisterDTO registerDTO);

    void refreshTokens(HttpServletRequest request, HttpServletResponse response)  throws IOException;

    boolean create(SoundEquipUser soundEquipUser);

    List<SoundEquipUserDTO> getAll();

    SoundEquipUserDTO getOneById(Long id);

    SoundEquipUserDTO getOneByName(String name);

    boolean update(SoundEquipUser soundEquipUser);

    boolean updateFtpState(FtpStateDTO ftpStateDTO);

    boolean updateFtpCredentials(FtpCredentialsDTO ftpCredentialsDTO);

    boolean updateTokens(String username, String accessToken, String refreshToken);

    boolean checkAccessToken(String username, String accessToken) throws MismatchedTokenException;

    boolean checkRefreshToken(String username, String refreshToken) throws MismatchedTokenException;

    boolean addRole(String username, String roleName);

    boolean delete(Long id);

}
