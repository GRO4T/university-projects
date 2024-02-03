package com.blueincorp.bluedrive.service;

import com.blueincorp.bluedrive.document.User;
import com.blueincorp.bluedrive.requestbody.AuthenticationResponse;
import com.blueincorp.bluedrive.requestbody.LoginRequest;
import com.blueincorp.bluedrive.requestbody.RegisterRequest;
import com.blueincorp.bluedrive.repository.UserRepository;
import lombok.AllArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
@AllArgsConstructor
public class AuthService {

    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;
    private final AuthenticationManager authenticationManager;
    private final JwtProvider jwtProvider;

    @Transactional
    public ResponseEntity signup(RegisterRequest registerRequest) {
        if(userRepository.findByUsername(registerRequest.getUsername()).isPresent())  {
            return new ResponseEntity("User with provided username already exists!", HttpStatus.CONFLICT);
        }
        if(userRepository.findByEmail(registerRequest.getEmail()).isPresent()) {
            return new ResponseEntity("User with provided email address already exists!", HttpStatus.CONFLICT);
        }

        User user = new User();
        user.setUsername(registerRequest.getUsername());
        user.setEmail(registerRequest.getEmail());
        user.setPassword(encodePassword(registerRequest.getPassword()));
        userRepository.save(user);

        return new ResponseEntity(HttpStatus.OK);
    }

    public AuthenticationResponse login(LoginRequest loginRequest) {
        var user = userRepository.findByUsername(loginRequest.getUsername());
        if (user.isPresent()) {
            if(!passwordMatches(loginRequest.getPassword(), user.get())) {
                return new AuthenticationResponse("", loginRequest.getUsername(), false);
            }

            Authentication authenticate = authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(loginRequest.getUsername(),
                        loginRequest.getPassword()));
            SecurityContextHolder.getContext().setAuthentication(authenticate);
            String authenticationToken = jwtProvider.generateToken(authenticate);
            return new AuthenticationResponse(authenticationToken, loginRequest.getUsername(), true);
        }

        return new AuthenticationResponse("", loginRequest.getUsername(), false);
    }

    public ResponseEntity validateToken(String token) {
        if (jwtProvider.validateToken(token)) {
            return new ResponseEntity(HttpStatus.OK);
        }
        else {
            return new ResponseEntity(HttpStatus.UNAUTHORIZED);
        }
    }

    public List<User> listUsers() {
        return userRepository.findAll();
    }

    private String encodePassword(String password) {
        return passwordEncoder.encode(password);
    }

    private boolean passwordMatches(String password, User user) {
        return passwordEncoder.matches(password, user.getPassword());
    }
}