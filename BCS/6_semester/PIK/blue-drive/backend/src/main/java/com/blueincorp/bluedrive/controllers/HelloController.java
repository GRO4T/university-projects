package com.blueincorp.bluedrive.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;


@Controller
public class HelloController {

   @GetMapping("api/hello")
   @ResponseBody
   public String hello(@RequestParam(name = "name", defaultValue = "World") String name){
       return String.format("Hello %s!", name);
   }

}
