package com.blueincorp.bluedrive.controllers;

import com.blueincorp.bluedrive.document.User;
import com.blueincorp.bluedrive.requestbody.LoginRequest;
import com.blueincorp.bluedrive.requestbody.RegisterRequest;
import com.blueincorp.bluedrive.service.AuthService;
import lombok.AllArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/auth")
@AllArgsConstructor
public class AuthController {

    private final AuthService authService;

    @PostMapping("/signup")
    public ResponseEntity signup(@RequestBody RegisterRequest registerRequest) {
        return authService.signup(registerRequest);
    }

    @PostMapping("/login")
    public ResponseEntity login(@RequestBody LoginRequest loginRequest) {
        var response =  authService.login(loginRequest);
        if(response.isAuthenticated()) {
            return new ResponseEntity(response, HttpStatus.OK);
        }
        else
            return new ResponseEntity("Incorrect username and/or password!", HttpStatus.UNAUTHORIZED);
    }

    @PostMapping("/validate-token")
    public ResponseEntity validateToken(@RequestBody String token) {
        return authService.validateToken(token);
    }

    @GetMapping("/debug/list-users")
    public ResponseEntity listUsers() {
        List<User> users = authService.listUsers();
        return new ResponseEntity(users, HttpStatus.OK);
    }
}