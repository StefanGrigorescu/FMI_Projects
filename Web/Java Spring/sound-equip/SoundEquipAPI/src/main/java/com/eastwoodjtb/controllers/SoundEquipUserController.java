package com.eastwoodjtb.controllers;

import com.eastwoodjtb.models.dto.AddRoleDTO;
import com.eastwoodjtb.models.dto.FtpCredentialsDTO;
import com.eastwoodjtb.models.dto.FtpStateDTO;
import com.eastwoodjtb.models.dto.SoundEquipUserDTO;
import com.eastwoodjtb.models.entities.SoundEquipUser;
import com.eastwoodjtb.services.SoundEquipUserService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import java.net.URI;
import java.util.List;


@RestController
@RequestMapping(value= "/sound-equip-user")
@RequiredArgsConstructor
@Slf4j
public class SoundEquipUserController {
    private final SoundEquipUserService soundEquipUserService;


    @PostMapping("/create")
    public ResponseEntity<Boolean> create(@RequestBody SoundEquipUser soundEquipUser)
    {
        log.info("SoundEquipUser Controller: create.");
        URI uri = URI.create(ServletUriComponentsBuilder.fromCurrentContextPath().path("/sound-equip-user/create").toUriString());
        return ResponseEntity.created(uri).body(soundEquipUserService.create(soundEquipUser));
    }

    @GetMapping("/get-all")
    public ResponseEntity<List<SoundEquipUserDTO>> getAll()
    {
        log.info("SoundEquipUser Controller: get all.");
        return ResponseEntity.ok().body(soundEquipUserService.getAll());
    }

    @GetMapping("/get-one/id")
    public ResponseEntity<SoundEquipUserDTO> getOneById(@RequestParam(value = "id"/*, defaultValue = "1"*/) Long id)
    {
        log.info("SoundEquipUser Controller: get one by id.");
        return ResponseEntity.ok().body(soundEquipUserService.getOneById(id));
    }

    @GetMapping("/get-one/name")
    public ResponseEntity<SoundEquipUserDTO> getOneByName(@RequestParam(value="name") String name)
    {
        log.info("SoundEquipUser Controller: get one by name.");
        return ResponseEntity.ok().body(soundEquipUserService.getOneByName(name));
    }

    @PutMapping("/update")
    public ResponseEntity<Boolean> update(@RequestBody SoundEquipUser soundEquipUser)
    {
        log.info("SoundEquipUser Controller: update.");
        return ResponseEntity.ok().body(soundEquipUserService.update(soundEquipUser));
    }

    @PutMapping("/update-ftp-state")
    public ResponseEntity<Boolean> updateFtpState(@RequestBody FtpStateDTO ftpStateDTO)
    {
        log.info("SoundEquipUser Controller: update ftp state.");
        return ResponseEntity.ok().body(soundEquipUserService.updateFtpState(ftpStateDTO));
    }

    @PutMapping("/update-ftp-credentials")
    public ResponseEntity<Boolean> updateFtpCredentials(@RequestBody FtpCredentialsDTO ftpCredentialsDTO)
    {
        log.info("SoundEquipUser Controller: update ftp credentials.");
        return ResponseEntity.ok().body(soundEquipUserService.updateFtpCredentials(ftpCredentialsDTO));
    }

    @PutMapping("/update/add-role")
    public ResponseEntity<Boolean> addRole(@RequestBody AddRoleDTO addRoleDTO)
    {
        log.info("SoundEquipUser Controller: add role.");
        return ResponseEntity.ok().body(soundEquipUserService.addRole(addRoleDTO.getUsername(), addRoleDTO.getRoleName()));
    }

    @DeleteMapping("/delete/id")
    public ResponseEntity<Boolean> delete(@RequestParam(value="id") Long id)
    {
        log.info("SoundEquipUser Controller: delete.");
        return ResponseEntity.ok().body(soundEquipUserService.delete(id));
    }

}
