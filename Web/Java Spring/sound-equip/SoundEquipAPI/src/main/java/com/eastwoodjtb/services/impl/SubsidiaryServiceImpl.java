package com.eastwoodjtb.services.impl;

import com.eastwoodjtb.models.dto.SubsidiaryDTO;
import com.eastwoodjtb.models.entities.Subsidiary;
import com.eastwoodjtb.repositories.ProductRepository;
import com.eastwoodjtb.repositories.SubsidiaryRepository;
import com.eastwoodjtb.services.SubsidiaryService;
import com.eastwoodjtb.utils.ConverterUtil;
import com.eastwoodjtb.utils.ListAccesser;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;
import java.util.List;
import java.util.stream.Collectors;

import static org.springframework.beans.BeanUtils.copyProperties;


@Service
@RequiredArgsConstructor
@Transactional
@Slf4j
public class SubsidiaryServiceImpl implements SubsidiaryService {
    private final SubsidiaryRepository subsidiaryRepository;
    private final ProductRepository productRepository;


    @Override
    public boolean create(SubsidiaryDTO subsidiaryDTO) {
        subsidiaryRepository.save(toEntity(subsidiaryDTO));
        return true;
    }

    @Override
    public List<SubsidiaryDTO> getAll() {
        return subsidiaryRepository.findAll()
                .stream()
                .map(this::toDTO)
                .collect(Collectors.toList());
    }

    @Override
    public SubsidiaryDTO getOneById(Long id) {
        return toDTO(subsidiaryRepository.findById(id).orElse(null));
    }

    @Override
    public SubsidiaryDTO getOneByName(String name)
    {
        List<SubsidiaryDTO> subsidiaryDTOList =
                subsidiaryRepository.findByName(name)
                        .stream()
                        .map(this::toDTO).toList();

        return ListAccesser.getOnlyOrNull(subsidiaryDTOList);
    }

    @Override
    public boolean update(SubsidiaryDTO subsidiaryDTO) {
        subsidiaryRepository.save(toEntity(subsidiaryDTO));
        return true;
    }

    @Override
    public boolean delete(Long id) {
        if (!subsidiaryRepository.existsById(id))
            return false;

        subsidiaryRepository.deleteById(id);
        return true;
    }

    private SubsidiaryDTO toDTO(Subsidiary entity)
    {
        if(entity == null)
            return null;

        SubsidiaryDTO dto = new SubsidiaryDTO();
        copyProperties(entity, dto, "products");
        dto.setProductIds(ConverterUtil.toIdList(entity.getProducts()));
        return dto;
    }

    private Subsidiary toEntity(SubsidiaryDTO dto)
    {
        if(dto == null)
            return null;

        Subsidiary entity = new Subsidiary();
        copyProperties(dto, entity, "productIds");
        entity.setProducts(ConverterUtil.toEntityList(dto.getProductIds(), productRepository));
        return entity;
    }

}
