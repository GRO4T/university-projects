package com.blueincorp.bluedrive.repository;

import com.blueincorp.bluedrive.document.User;
import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.Optional;

public interface UserRepository extends MongoRepository<User, Long> {
    Optional<User> findByUsername(String username);
    Optional<User> findByEmail(String username);
}
