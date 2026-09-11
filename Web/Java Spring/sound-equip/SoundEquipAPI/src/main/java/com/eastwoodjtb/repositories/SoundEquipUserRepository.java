package com.eastwoodjtb.repositories;

import com.eastwoodjtb.models.entities.SoundEquipUser;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;


public interface SoundEquipUserRepository extends JpaRepository<SoundEquipUser, Long> {
    List<SoundEquipUser> findByName(String name);

}
