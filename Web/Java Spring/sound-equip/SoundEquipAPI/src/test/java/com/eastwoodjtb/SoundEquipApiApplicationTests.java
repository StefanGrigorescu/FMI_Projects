package com.eastwoodjtb;

import com.eastwoodjtb.configs.ModelMapperConfig;
import com.eastwoodjtb.configs.SpringFoxConfig;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.support.AnnotationConfigContextLoader;

//@SpringBootTest
@ContextConfiguration(
        classes = {SpringFoxConfig.class, ModelMapperConfig.class},
        loader = AnnotationConfigContextLoader.class
)
class SoundEquipApiApplicationTests {

    @Test
    void contextLoads() {
    }

}
