package com.eastwoodjtb;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;


@SpringBootApplication
public class SoundEquipApiApplication {
    private static final Logger logger = LoggerFactory.getLogger(SoundEquipApiApplication.class);

    public static void main(String[] args) {
        try {
            SpringApplication.run(SoundEquipApiApplication.class, args);
        }
        catch (Exception e)
        {
            logger.error("An exception was encountered while running the application: {}.", e.getMessage());
        }
    }

    @Bean
    PasswordEncoder passwordEncoder()
    {
        return new BCryptPasswordEncoder();
    }

    /*@Bean
    WebClient.Builder getWebClientBuilder()
    {
        return WebClient.builder();
    }*/

}
