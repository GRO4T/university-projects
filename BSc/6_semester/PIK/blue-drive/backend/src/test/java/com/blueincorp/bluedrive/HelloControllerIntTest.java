package com.blueincorp.bluedrive;

import com.blueincorp.bluedrive.controllers.HelloController;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.test.context.junit.jupiter.SpringExtension;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;

import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;

@ExtendWith(SpringExtension.class)
@WebMvcTest(HelloController.class)
class HelloControllerIntTest {

    @Autowired
    private MockMvc mvc;

    @Test
    void testHello() throws Exception {
        mvc.perform(MockMvcRequestBuilders.get("/api/hello"))
                .andExpect(content().string("Hello World!"));
    }

    @Test
    void testHelloWithParam() throws Exception {
        mvc.perform(MockMvcRequestBuilders.get("/api/hello?name=Dan"))
                .andExpect(content().string("Hello Dan!"));
    }

}