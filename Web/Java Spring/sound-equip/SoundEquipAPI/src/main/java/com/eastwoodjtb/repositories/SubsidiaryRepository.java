package com.eastwoodjtb.repositories;

import com.eastwoodjtb.models.entities.Subsidiary;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;


public interface SubsidiaryRepository  extends JpaRepository<Subsidiary, Long> {
    List<Subsidiary> findByName(String name);

}
