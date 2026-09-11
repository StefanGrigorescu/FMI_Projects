package com.eastwoodjtb.services.impl;

import com.eastwoodjtb.models.entities.Role;
import com.eastwoodjtb.repositories.RoleRepository;
import com.eastwoodjtb.services.RoleService;
import com.eastwoodjtb.utils.ListAccesser;
import com.eastwoodjtb.utils.LogMarkers;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.dao.DataIntegrityViolationException;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;
import java.util.List;

import static java.text.MessageFormat.format;


@Service
@RequiredArgsConstructor
@Transactional
@Slf4j
public class RoleServiceImpl implements RoleService {
    private final RoleRepository roleRepository;


    @Override
    public boolean create(Role role) {
        Role existingRole;
        try {
            existingRole = ListAccesser.getOnlyOrNull(roleRepository.findByName(role.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("RoleServiceImpl.create: Two roles with the same name (name = {0}) were found in the database! Prompt action is required!", role.getName()));
            return false;
        }
        if (existingRole != null) {
            log.error("RoleServiceImpl.create: Attempting to insert an already existing role! Try a different role name.");
            return false;
        }
        roleRepository.save(role);
        return true;
    }

    @Override
    public List<Role> getAll() {
        return roleRepository.findAll();
    }

    @Override
    public Role getOneById(Long id) {
        return roleRepository.getById(id);
    }

    @Override
    public Role getOneByName(String name) {
        try {
            return ListAccesser.getOnlyOrNull(roleRepository.findByName(name));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("RoleServiceImpl.getOneByName: Two roles with the same name (name = {0}) were found in the database! Prompt action is required!", name));
            return null;
        }
    }

    @Override
    public boolean update(Role role) {
        Role existingRole;
        try {
            existingRole = ListAccesser.getOnlyOrNull(roleRepository.findByName(role.getName()));
        }
        catch (DataIntegrityViolationException e)
        {
            log.error(LogMarkers.fatalMarker, format("RoleServiceImpl.update: Two roles with the same name (name = {0}) were found in the database! Prompt action is required!", role.getName()));
            return false;
        }
        if (existingRole == null) {
            log.warn("RoleServiceImpl.update: Attempting to update a non existing role!");
            return false;
        }
        roleRepository.save(role);
        return true;
    }

    @Override
    public boolean delete(Long id) {
        if(!roleRepository.existsById(id))
        {
            log.warn("SoundEquipUserServiceImpl.delete: No role with id = {} was found in the database.", id);
            return false;
        }
        roleRepository.deleteById(id);
        log.info("RoleServiceImpl.delete: Deleted the role with id = {}.", id);
        return true;
    }

}
