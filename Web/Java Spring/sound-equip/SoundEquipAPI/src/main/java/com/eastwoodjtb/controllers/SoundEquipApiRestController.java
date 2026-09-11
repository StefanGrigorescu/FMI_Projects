package com.eastwoodjtb.controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.stereotype.Component;
import org.springframework.web.bind.annotation.*;

@RestController
@EnableAutoConfiguration
@Component
public class SoundEquipApiRestController
{

    @Autowired
    public SoundEquipApiRestController()
    {
    }


}
