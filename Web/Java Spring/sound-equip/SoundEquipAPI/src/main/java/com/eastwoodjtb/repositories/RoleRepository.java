package com.eastwoodjtb.repositories;

import com.eastwoodjtb.models.entities.Role;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;


public interface RoleRepository extends JpaRepository<Role, Long> {
    List<Role> findByName(String name);

}
