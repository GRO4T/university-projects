package com.blueincorp.bluedrive.requestbody;

import lombok.Builder;
import lombok.Getter;
import lombok.Setter;

import java.util.Date;

@Getter
@Setter
@Builder
public class FileInfo {
    private String filename;
    private String directoryPath;
    private String id;
    private Date lastModified;
    private Long size;
    private String contentType;
    private String type;
}
