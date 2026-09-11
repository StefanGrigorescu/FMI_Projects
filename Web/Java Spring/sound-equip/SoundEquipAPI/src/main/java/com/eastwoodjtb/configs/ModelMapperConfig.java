package com.eastwoodjtb.configs;

import org.modelmapper.ModelMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;


@Configuration
public class ModelMapperConfig {
    private final Logger logger = LoggerFactory.getLogger(ModelMapperConfig.class);

    @Bean
    public ModelMapper modelMapper() {
        logger.info("ModelMapperConfig:  Used for context configuration.");
        return new ModelMapper();
    }

}
