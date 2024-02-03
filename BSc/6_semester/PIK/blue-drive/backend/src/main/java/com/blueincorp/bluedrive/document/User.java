package com.blueincorp.bluedrive.document;

import lombok.*;
import org.springframework.data.annotation.Transient;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.annotation.Id;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Builder
@Document
public class User {
    @Transient
    public static final String SEQUENCE_NAME = "user_sequence";

    @Id
    private String userID;
    private String username;
    private String email;
    private String password;
}
