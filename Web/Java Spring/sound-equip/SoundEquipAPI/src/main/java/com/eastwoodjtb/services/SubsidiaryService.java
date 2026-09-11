package com.eastwoodjtb.services;

import com.eastwoodjtb.models.dto.SubsidiaryDTO;

import java.util.List;


public interface SubsidiaryService {
    boolean create(SubsidiaryDTO subsidiaryDTO);

    List<SubsidiaryDTO> getAll();

    SubsidiaryDTO getOneById(Long id);

    SubsidiaryDTO getOneByName(String name);

    boolean update(SubsidiaryDTO subsidiaryDTO);

    boolean delete(Long id);
}
