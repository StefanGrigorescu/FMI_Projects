package com.eastwoodjtb.services.impl;

import com.eastwoodjtb.models.dto.ProductDTO;
import com.eastwoodjtb.models.dto.ProductJoinedDTO;
import com.eastwoodjtb.models.entities.Product;
import com.eastwoodjtb.models.entities.Subsidiary;
import com.eastwoodjtb.repositories.ProductRepository;
import com.eastwoodjtb.repositories.SubsidiaryRepository;
import com.eastwoodjtb.services.ProductService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

import static org.springframework.beans.BeanUtils.copyProperties;


@Service
@RequiredArgsConstructor
@Transactional
@Slf4j
public class ProductServiceImpl implements ProductService {
    private final ProductRepository productRepository;
    private final SubsidiaryRepository subsidiaryRepository;


    @Override
    public boolean create(ProductDTO productDTO)
    {
        productRepository.save(toEntity(productDTO));
        return true;
    }

    @Override
    public List<ProductDTO> getAll()
    {
        return productRepository.findAll()
                .stream()
                .map(this::toDTO)
                .collect(Collectors.toList());
    }

    @Override
    public ProductDTO getOneById(Long id)
    {
        return toDTO(productRepository.findById(id).orElse(null));
    }

    @Override
    public List<ProductDTO> getAllByName(String name)
    {
        return productRepository.findByName(name)
                        .stream()
                        .map(this::toDTO).toList();
    }

    @Override
    public List<ProductJoinedDTO> getAllJoined()
    {
        return productRepository.findAll()
                .stream()
                .map(this::toJoinedDTO)
                .collect(Collectors.toList());
    }

    @Override
    public ProductJoinedDTO getOneByIdJoined(Long id)
    {
        return toJoinedDTO(productRepository.findById(id).orElse(null));
    }

    @Override
    public List<ProductJoinedDTO> getAllByNameJoined(String name)
    {
        return productRepository.findByName(name)
                        .stream()
                        .map(this::toJoinedDTO).toList();
    }

    @Override
    public List<ProductJoinedDTO> getAllJoinedSorted(Comparator<ProductJoinedDTO> comparator)
    {
        return productRepository.findAll()
                .stream()
                .map(this::toJoinedDTO).sorted(comparator)
                .collect(Collectors.toList());
    }

    @Override
    public boolean update(ProductDTO productDTO)
    {
        productRepository.save(toEntity(productDTO));
        return true;
    }

    @Override
    public boolean delete(Long id) {
        if (!productRepository.existsById(id))
            return false;

        productRepository.deleteById(id);
        return true;
    }

    private ProductDTO toDTO(Product entity)
    {
        if(entity == null)
            return null;

        ProductDTO dto = new ProductDTO();
        copyProperties(entity, dto, "subsidiary");
        dto.setSubsidiaryId(entity.getSubsidiary().getId());
        return dto;
    }

    private Product toEntity(ProductDTO dto)
    {
        if(dto == null)
            return null;

        Product entity = new Product();
        copyProperties(dto, entity, "subsidiaryId");
        entity.setSubsidiary(subsidiaryRepository.getById(dto.getSubsidiaryId()));
        return entity;
    }

    private ProductJoinedDTO toJoinedDTO(Product entity)
    {
        if(entity == null)
            return null;

        ProductJoinedDTO productJoined = new ProductJoinedDTO();
        copyProperties(entity, productJoined, "subsidiary");
        Subsidiary subsidiary = entity.getSubsidiary();

        productJoined.setSubsidiaryId(subsidiary.getId());
        productJoined.setSubsidiaryName(subsidiary.getName());
        productJoined.setSubsidiaryCountryCode(subsidiary.getCountryCode());
        return productJoined;
    }

}
