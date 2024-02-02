package com.blueincorp.bluedrive.service;

import com.blueincorp.bluedrive.requestbody.FileInfo;
import org.javaswift.joss.client.factory.AccountFactory;
import org.javaswift.joss.client.factory.AuthenticationMethod;
import org.javaswift.joss.model.Account;
import org.javaswift.joss.model.Container;
import org.javaswift.joss.model.StoredObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@Service
public class OpenstackSwiftService {
    private Logger logger = LoggerFactory.getLogger(OpenstackSwiftService.class);

    private static final String SWIFT_STORAGE_URL = System.getenv("SWIFT_STORAGE_URL");
    private static final String SWIFT_STORAGE_USER = System.getenv("SWIFT_STORAGE_USER");
    private static final String SWIFT_STORAGE_PWD = System.getenv("SWIFT_STORAGE_PWD");
    private static final String SWIFT_STORAGE_TENANT = "tester";

    private Account loginToSwift() {
        return new AccountFactory()
                .setAuthUrl(SWIFT_STORAGE_URL)
                .setUsername(SWIFT_STORAGE_USER)
                .setPassword(SWIFT_STORAGE_PWD)
                .setTenantName(SWIFT_STORAGE_TENANT)
                .setAuthenticationMethod(AuthenticationMethod.BASIC)
                .createAccount();
    }

    private String generateID() {
        return java.util.UUID.randomUUID().toString();
    }

    private Container getContainer(String containerName) {
        final Container container = loginToSwift().getContainer(containerName);
        if (!container.exists()) {
            container.create();
        }
        return container;
    }

    public String uploadFile(byte[] newFile, String containerName) {
        final String swiftID = generateID();
        var container = getContainer(containerName);
        final StoredObject newFileObject = container.getObject(swiftID);
        if (!newFileObject.exists()) {
            newFileObject.uploadObject(newFile);
        }
        return swiftID;
    }

    public Optional<FileInfo> getMeta(String swiftGeneratedFilename, String containerName) {
        var container = getContainer(containerName);
        var storedObject = container.getObject(swiftGeneratedFilename);
        if (!storedObject.exists()) return Optional.empty();
        return Optional.of(FileInfo.builder()
                .id(storedObject.getName())
                .lastModified(storedObject.getLastModifiedAsDate())
                .contentType(storedObject.getContentType())
                .size(storedObject.getContentLength())
                .build());
    }

    public byte[] download(String id, String containerName) {
        var container = getContainer(containerName);
        var storedObject = container.getObject(id);
        if (!storedObject.exists())
            throw new OpenstackServiceException("File does not exist");
        return storedObject.downloadObject();
    }

    public boolean delete (String swiftID, String containerName) {
        var container = getContainer(containerName);
        var storedObject = container.getObject(swiftID);
        if (!storedObject.exists()) return false;
        storedObject.delete();
        return true;
    }

    public List<FileInfo> listContainer(String containerName) {
        var container = getContainer(containerName);
        return container.list().stream().map(storedObject -> FileInfo.builder()
                .id(storedObject.getName())
                .lastModified(storedObject.getLastModifiedAsDate())
                .contentType(storedObject.getContentType())
                .size(storedObject.getContentLength())
                .build()).collect(Collectors.toList());
    }

    public void deleteAll() {
        for (var container : loginToSwift().list())
        {
            container.list().stream().forEach(storedObject -> storedObject.delete());
        }
    }

    public class OpenstackServiceException extends RuntimeException {
        public OpenstackServiceException(String errorMsg) {
            super(errorMsg);
        }
    }
}
