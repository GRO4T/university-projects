package com.blueincorp.bluedrive.document;

import lombok.Builder;
import lombok.Getter;
import lombok.Setter;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.Document;

import javax.validation.constraints.NotNull;

@Getter
@Setter
@Builder
@Document
public class File {
    @Id
    private String swiftID;
    private String filename;
    @NotNull
    private String directoryPath;
    @NotNull
    private String containerName;
    @NotNull
    private String type;
}
