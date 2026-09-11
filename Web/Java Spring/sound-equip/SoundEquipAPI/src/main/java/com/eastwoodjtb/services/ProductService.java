package com.eastwoodjtb.services;

import com.eastwoodjtb.models.dto.ProductDTO;
import com.eastwoodjtb.models.dto.ProductJoinedDTO;

import java.util.Comparator;
import java.util.List;


public interface ProductService {
    boolean create(ProductDTO productDTO);

    List<ProductDTO> getAll();

    ProductDTO getOneById(Long id);

    List<ProductDTO> getAllByName(String name);

    List<ProductJoinedDTO> getAllJoined();

    ProductJoinedDTO getOneByIdJoined(Long id);

    List<ProductJoinedDTO> getAllByNameJoined(String name);

    List<ProductJoinedDTO> getAllJoinedSorted(Comparator<ProductJoinedDTO> comparator);

    boolean update(ProductDTO productDTO);

    boolean delete(Long id);

}
