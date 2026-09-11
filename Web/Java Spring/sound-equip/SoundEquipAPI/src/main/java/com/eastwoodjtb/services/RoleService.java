package com.eastwoodjtb.services;

import com.eastwoodjtb.models.entities.Role;

import java.util.List;


public interface RoleService {
    boolean create(Role role);

    List<Role> getAll();

    Role getOneById(Long id);

    Role getOneByName(String name);

    boolean update(Role role);

    boolean delete(Long id);

}
