package com.eastwoodjtb.controllers;

import com.eastwoodjtb.models.dto.SubsidiaryDTO;
import com.eastwoodjtb.services.SubsidiaryService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;


@RestController
@RequestMapping(value = "/subsidiary")
@RequiredArgsConstructor
@Slf4j
public class SubsidiaryController {
    private final SubsidiaryService subsidiaryService;


    @PostMapping("/create")
    public ResponseEntity<Boolean> create(@RequestBody SubsidiaryDTO subsidiaryDTO)
    {
        log.info("Subsidiary Controller: create.");
        URI uri = URI.create(ServletUriComponentsBuilder.fromCurrentContextPath().path("/subsidiary/create").toUriString());
        return ResponseEntity.created(uri).body(subsidiaryService.create(subsidiaryDTO));
    }

    @GetMapping("/get-all")
    public ResponseEntity<List<SubsidiaryDTO>> getAll()
    {
        log.info("Subsidiary Controller: get all.");
        return ResponseEntity.ok().body(subsidiaryService.getAll());
    }

    @GetMapping("/get-one/id")
    public ResponseEntity<SubsidiaryDTO> getOneById(@RequestParam(value = "id"/*, defaultValue = "1"*/) Long id)
    {
        log.info("Subsidiary Controller: get one by id.");
        return ResponseEntity.ok().body(subsidiaryService.getOneById(id));
    }

    @GetMapping("/get-one/name")
    public ResponseEntity<SubsidiaryDTO> getOneByName(@RequestParam(value="name") String name)
    {
        log.info("Subsidiary Controller: get one by name.");
        return ResponseEntity.ok().body(subsidiaryService.getOneByName(name));
    }

    @PutMapping("/update")
    public ResponseEntity<Boolean> update(@RequestBody SubsidiaryDTO subsidiaryDTO)
    {
        log.info("Subsidiary Controller: update.");
        return ResponseEntity.ok().body(subsidiaryService.update(subsidiaryDTO));
    }

    @DeleteMapping("/delete/id")
    public ResponseEntity<Boolean> delete(@RequestParam(value="id") Long id)
    {
        log.info("Subsidiary Controller: delete.");
        return ResponseEntity.ok().body(subsidiaryService.delete(id));
    }

}
