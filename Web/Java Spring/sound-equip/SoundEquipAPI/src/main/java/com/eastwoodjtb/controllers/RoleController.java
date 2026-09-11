package com.eastwoodjtb.controllers;

import com.eastwoodjtb.models.entities.Role;
import com.eastwoodjtb.services.RoleService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;


@RestController
@RequestMapping(value= "/role")
@RequiredArgsConstructor
@Slf4j
public class RoleController {
    private final RoleService roleService;


    @PostMapping("/create")
    public ResponseEntity<Boolean> create(@RequestBody Role role)
    {
        log.info("Role Controller: create.");
        URI uri = URI.create(ServletUriComponentsBuilder.fromCurrentContextPath().path("/role/create").toUriString());
        return ResponseEntity.created(uri).body(roleService.create(role));
    }

    @GetMapping("/get-all")
    public ResponseEntity<List<Role>> getAll()
    {
        log.info("Role Controller: get all.");
        return ResponseEntity.ok().body(roleService.getAll());
    }

    @GetMapping("/get-one/id")
    public ResponseEntity<Role> getOneById(@RequestParam(value = "id"/*, defaultValue = "1"*/) Long id)
    {
        log.info("Role Controller: get one by id.");
        return ResponseEntity.ok().body(roleService.getOneById(id));
    }

    @GetMapping("/get-one/name")
    public ResponseEntity<Role> getOneByName(@RequestParam(value="name") String name)
    {
        log.info("Role Controller: get one by name.");
        return ResponseEntity.ok().body(roleService.getOneByName(name));
    }

    @PutMapping("/update")
    public ResponseEntity<Boolean> update(@RequestBody Role role)
    {
        log.info("Role Controller: update.");
        return ResponseEntity.ok().body(roleService.update(role));
    }

    @DeleteMapping("/delete/id")
    public ResponseEntity<Boolean> delete(@RequestParam(value="id") Long id)
    {
        log.info("Role Controller: delete.");
        return ResponseEntity.ok().body(roleService.delete(id));
    }

}
