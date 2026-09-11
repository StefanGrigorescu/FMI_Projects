package com.eastwoodjtb.controllers;

import com.eastwoodjtb.models.dto.ProductDTO;
import com.eastwoodjtb.models.dto.ProductJoinedDTO;
import com.eastwoodjtb.services.ProductService;
import com.eastwoodjtb.utils.Comparators;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;

@RestController
@RequestMapping(value= "/product")
@RequiredArgsConstructor
@Slf4j
public class ProductController {
    private final ProductService productService;


    @PostMapping("/create")
    public ResponseEntity<Boolean> create(@RequestBody ProductDTO productDTO)
    {
        log.info("Product Controller: create.");
        URI uri = URI.create(ServletUriComponentsBuilder.fromCurrentContextPath().path("/product/create").toUriString());
        return ResponseEntity.created(uri).body(productService.create(productDTO));
    }

    @GetMapping("/get-all")
    public ResponseEntity<List<ProductDTO>> getAll()
    {
        log.info("Product Controller: get all.");
        return ResponseEntity.ok().body(productService.getAll());
    }

    @GetMapping("/get-one/id")
    public ResponseEntity<ProductDTO> getOneById(@RequestParam(value = "id", defaultValue = "1") Long id)
    {
        log.info("Product Controller: get one by id.");
        return ResponseEntity.ok().body(productService.getOneById(id));
    }

    @GetMapping("/get-all/name")
    public ResponseEntity<List<ProductDTO>> getAllByName(@RequestParam(value="name") String name)
    {
        log.info("Product Controller: get all by name.");
        return ResponseEntity.ok().body(productService.getAllByName(name));
    }

    @GetMapping(value = "/joined/get-all")
    public ResponseEntity<List<ProductJoinedDTO>> getAllJoined()
    {
        log.info("Product Controller: get all joined.");
        return ResponseEntity.ok().body(productService.getAllJoined());
    }

    @GetMapping("/joined/get-one/id")
    public ResponseEntity<ProductJoinedDTO> getOneByIdJoined(@RequestParam(value = "id", defaultValue = "1") Long id)
    {
        log.info("Product Controller: get all by id joined.");
        return ResponseEntity.ok().body(productService.getOneByIdJoined(id));
    }

    @GetMapping("/joined/get-all/name")
    public ResponseEntity<List<ProductJoinedDTO>> getAllByNameJoined(@RequestParam(value="name") String name)
    {
        log.info("Product Controller: get all by name joined.");
        return ResponseEntity.ok().body(productService.getAllByNameJoined(name));
    }

    @GetMapping(value = "/joined/get-all/sorted/by-name")
    public ResponseEntity<List<ProductJoinedDTO>> getAllJoinedSortedByName()
    {
        log.info("Product Controller: get all joined sorted by name.");
        return ResponseEntity.ok().body(productService.getAllJoinedSorted(Comparators.byName));
    }

    @GetMapping(value = "/joined/get-all/sorted/by-subsidiary-name")
    public ResponseEntity<List<ProductJoinedDTO>> getAllJoinedSortedBySubsidiaryName()
    {
        log.info("Product Controller: get all joined sorted by subsidiary name.");
        return ResponseEntity.ok().body(productService.getAllJoinedSorted(Comparators.bySubsidiaryName));
    }

    @GetMapping(value = "/joined/get-all/sorted/by-subsidiary-country-code")
    public ResponseEntity<List<ProductJoinedDTO>> getAllJoinedSortedBySubsidiaryCountryCode()
    {
        log.info("Product Controller: get all joined sorted by subsidiary country code.");
        return ResponseEntity.ok().body(productService.getAllJoinedSorted(Comparators.bySubsidiaryCountryCode));
    }

    @GetMapping(value = "/joined/get-all/sorted/by-quantity")
    public ResponseEntity<List<ProductJoinedDTO>> getAllJoinedSortedByQuantity()
    {
        log.info("Product Controller: get all joined sorted by quantity.");
        return ResponseEntity.ok().body(productService.getAllJoinedSorted(Comparators.byQuantity));
    }

    @PutMapping("/update")
    public ResponseEntity<Boolean> update(@RequestBody ProductDTO productDTO)
    {
        log.info("Product Controller: update.");
        return ResponseEntity.ok().body(productService.update(productDTO));
    }

    @DeleteMapping("/delete/id")
    public ResponseEntity<Boolean> delete(@RequestParam(value = "id") Long id)
    {
        log.info("Product Controller: delete.");
        return ResponseEntity.ok().body(productService.delete(id));
    }

}
