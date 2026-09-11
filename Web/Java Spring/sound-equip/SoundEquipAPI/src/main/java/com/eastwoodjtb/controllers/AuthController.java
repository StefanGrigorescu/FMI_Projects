package com.eastwoodjtb.controllers;

import com.eastwoodjtb.models.dto.RegisterDTO;
import com.eastwoodjtb.services.SoundEquipUserService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.net.URI;


@RestController
@RequestMapping(value= "/auth")
@RequiredArgsConstructor
@Slf4j
public class AuthController {
    private final SoundEquipUserService soundEquipUserService;


    @PostMapping("/register")
    public ResponseEntity<Boolean> register(@RequestBody RegisterDTO registerDTO)
    {
        log.info("Auth Controller: register.");
        URI uri = URI.create(ServletUriComponentsBuilder.fromCurrentContextPath().path("/auth/register").toUriString());
        return ResponseEntity.created(uri).body(soundEquipUserService.register(registerDTO));
    }

    @GetMapping("/refresh-token")
    public void refreshTokens(HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        log.info("Auth Controller: refresh tokens.");
        soundEquipUserService.refreshTokens(request, response);
    }

    // login login request - but I think it already exists from spring.

}
