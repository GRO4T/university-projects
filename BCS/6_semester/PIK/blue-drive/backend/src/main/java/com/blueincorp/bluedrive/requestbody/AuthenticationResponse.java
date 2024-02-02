package com.blueincorp.bluedrive.requestbody;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;

@Data
@AllArgsConstructor
@Builder
public class AuthenticationResponse {
    private String authenticationToken;
    private String username;
    private boolean authenticated;
}