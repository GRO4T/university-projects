package com.blueincorp.bluedrive.repository;

import com.blueincorp.bluedrive.document.File;
import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.List;

public interface FileRepository extends MongoRepository<File, Long> {
    File findBySwiftIDAndContainerName(String swiftID, String containerName);
    File findBySwiftIDAndTypeAndContainerName(String swiftID, String type, String containerName);
    File findByFilenameAndDirectoryPathAndContainerName(String filename, String directoryPath, String containerName);
    File findByDirectoryPathAndContainerNameAndType(String directoryPath, String containerName, String type);
    List<File> findAllByDirectoryPathStartingWithAndContainerName(String directoryPathPrefix, String containerName);
    void deleteBySwiftIDAndContainerName(String swiftID, String containerName);
    void deleteBySwiftIDAndContainerNameAndType(String swiftID, String containerName, String type);
    void deleteByDirectoryPathAndTypeAndContainerName(String directoryPath, String type, String containerName);
}
