package com.blueincorp.bluedrive.service;

import org.springframework.security.core.userdetails.User;
import io.jsonwebtoken.Jwts;
import org.springframework.security.core.Authentication;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.io.IOException;
import java.io.InputStream;
import java.security.*;
import java.security.cert.CertificateException;

@Service
public class JwtProvider {
    private static final char[] JWT_KEYSTORE_PWD = System.getenv("JWT_KEYSTORE_PWD").toCharArray();
    private static final String JWT_KEYSTORE_FILE = "/bluedrive.jks";
    private static final String JWT_KEYSTORE_ALIAS = "bluedrive";

    private KeyStore keyStore;

    @PostConstruct
    public void init() {
        try {
            keyStore = KeyStore.getInstance("JKS");
            InputStream resourceAsStream = getClass().getResourceAsStream(JWT_KEYSTORE_FILE);
            keyStore.load(resourceAsStream, JWT_KEYSTORE_PWD);
        } catch (KeyStoreException | CertificateException | NoSuchAlgorithmException | IOException e) {
            throw new RuntimeException("Exception occurred while loading keystore");
        }
    }

    public boolean validateToken(String token) {
        try {
            Jwts.parser().setSigningKey(getPublicKey()).parseClaimsJws(token);
        }
        catch (Exception e) {
            return false;
        }
        return true;
    }

    public String generateToken(Authentication authentication) {
        User principal = (User)authentication.getPrincipal();
        return Jwts.builder()
                .setSubject(principal.getUsername())
                .signWith(getPrivateKey())
                .compact();
    }

    private PublicKey getPublicKey() {
        try {
            return keyStore.getCertificate(JWT_KEYSTORE_ALIAS).getPublicKey();
        } catch (KeyStoreException e) {
            throw new RuntimeException("Exception occurred while retrieving public key from keystore");
        }
    }

    private PrivateKey getPrivateKey() {
        try {
            return (PrivateKey)keyStore.getKey(JWT_KEYSTORE_ALIAS, JWT_KEYSTORE_PWD);
        } catch (KeyStoreException | NoSuchAlgorithmException | UnrecoverableKeyException e) {
            throw new RuntimeException("Exception occurred while retrieving private key from keystore");
        }
    }
}
