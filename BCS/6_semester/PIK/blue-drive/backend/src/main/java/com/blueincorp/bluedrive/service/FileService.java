package com.blueincorp.bluedrive.service;

import com.blueincorp.bluedrive.document.File;
import com.blueincorp.bluedrive.repository.FileRepository;
import com.blueincorp.bluedrive.requestbody.FileInfo;
import lombok.AllArgsConstructor;
import lombok.Getter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.ByteArrayResource;
import org.springframework.core.io.Resource;
import org.springframework.stereotype.Service;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class FileService {
    private final Logger logger = LoggerFactory.getLogger(FileService.class);

    private final FileRepository fileRepository;
    private final OpenstackSwiftService openstackSwiftService;

    private final String typeFile = "file";
    private final String typeDir = "directory";

    @Autowired
    public FileService(FileRepository fileRepository, OpenstackSwiftService openstackSwiftService) {
        this.fileRepository = fileRepository;
        this.openstackSwiftService = openstackSwiftService;
    }

    private String generateID() {
        return java.util.UUID.randomUUID().toString();
    }

    public void createFile(String filepath, String containerName, byte[] file){
        if (fileExists(filepath, containerName))
            throw new FileAlreadyExists(filepath);
        if (directoryExists(filepath, containerName))
            throw new DirectoryWithTheSameName(filepath);

        Path path = Paths.get(filepath);
        for (int i = 1; i < path.getNameCount(); i++) {
            var subpath = "/" + path.subpath(0, i).toString();
            if (subpath.equals("/")) continue;
            createDirectory(subpath, containerName);
        }
        var filename = path.getFileName().toString();
        var directoryPath = path.getParent().toString();
        final String id = openstackSwiftService.uploadFile(file, containerName);
        File newFile = File.builder()
                .swiftID(id)
                .filename(filename)
                .directoryPath(directoryPath)
                .containerName(containerName)
                .type(typeFile)
                .build();
        fileRepository.save(newFile);
    }

    public void createDirectory(String directoryPath, String containerName) {
        if (fileExists(directoryPath, containerName))
            throw new FileAlreadyExists(directoryPath);
        else if (!directoryExists(directoryPath, containerName)) {
            File newDirectory = File.builder()
                    .swiftID(generateID())
                    .directoryPath(directoryPath)
                    .containerName(containerName)
                    .type(typeDir)
                    .build();
            fileRepository.save(newDirectory);
        }
    }

    public FileInfo getMetadataByID(String id, String containerName) {
        var file = fileRepository.findBySwiftIDAndContainerName(id, containerName);
        return getFileMeta(file, containerName);
    }

    public List<FileInfo> listDirectory(String directoryPath, String containerName) {
        var fileList =
                fileRepository.findAllByDirectoryPathStartingWithAndContainerName(directoryPath, containerName);
        return fileList.stream().map(file -> {
            var metadataOptional = openstackSwiftService.getMeta(file.getSwiftID(), containerName);
            if (metadataOptional.isPresent()) {
                var metadata = metadataOptional.get();
                metadata.setFilename(file.getFilename());
                metadata.setDirectoryPath(file.getDirectoryPath());
                metadata.setType(file.getType());
                return metadata;
            }
            else {
               return FileInfo.builder()
                        .id(file.getSwiftID())
                        .directoryPath(file.getDirectoryPath())
                        .type(file.getType())
                        .build();
            }
        }).collect(Collectors.toList());
    }



    public void deleteFile(String id, String containerName) {
        if (!openstackSwiftService.delete(id, containerName))
            throw new FileDoesNotExist();
        fileRepository.deleteBySwiftIDAndContainerName(id, containerName);
    }

    public void deleteDirectory(String id, String containerName) {
        var directory = fileRepository.findBySwiftIDAndTypeAndContainerName(id, typeDir, containerName);
        if (directory == null)
            throw new DirectoryDoesNotExist();
        fileRepository.deleteBySwiftIDAndContainerNameAndType(id, containerName, typeDir);
        var filesInDirectory =
                fileRepository.findAllByDirectoryPathStartingWithAndContainerName(directory.getDirectoryPath(), containerName);
        for (var file : filesInDirectory) {
            fileRepository.deleteBySwiftIDAndContainerName(file.getSwiftID(), containerName);
        }
    }

    public NamedResource downloadFile(String id, String containerName) {
        var file = fileRepository.findBySwiftIDAndContainerName(id, containerName);
        if (file == null)
            throw new FileDoesNotExist();
        try {
            var resource = new ByteArrayResource(openstackSwiftService.download(id, containerName));
            return new NamedResource(file.getFilename(), resource);
        }
        catch (OpenstackSwiftService.OpenstackServiceException e) {
            throw new FileDoesNotExist();
        }
    }

    public void deleteAll() {
        fileRepository.deleteAll();
    }

    private FileInfo getFileMeta(File file, String containerName) {
        if (file == null) {
            throw new FileDoesNotExist();
        }
        if (file.getType().equals(typeFile)) {
            var metadataOptional = openstackSwiftService.getMeta(file.getSwiftID(), containerName);
            if (metadataOptional.isEmpty())
                throw new NotInSwiftStorage(file.getSwiftID());
            var metadata = metadataOptional.get();
            metadata.setFilename(file.getFilename());
            metadata.setDirectoryPath(file.getDirectoryPath());
            metadata.setType(file.getType());
            return metadata;
        }
        else {
            return FileInfo.builder()
                    .id(file.getSwiftID())
                    .directoryPath(file.getDirectoryPath())
                    .type(file.getType())
                    .build();
        }
    }

    public boolean directoryExists(String path, String containerName) {
        var directory = fileRepository.findByDirectoryPathAndContainerNameAndType(path, containerName, typeDir);
        return directory != null;
    }

    public boolean directoryExistsByID(String id, String containerName) {
        var directory = fileRepository.findBySwiftIDAndTypeAndContainerName(id, typeDir, containerName);
        return directory != null;
    }

    public boolean fileExists(String filepath, String containerName) {
        Path p = Paths.get(filepath);
        var filename = p.getFileName().toString();
        var directoryPath = p.getParent().toString();
        if (directoryPath == null)
            return false;
        var file = fileRepository.
                findByFilenameAndDirectoryPathAndContainerName(filename, directoryPath, containerName);
        return file != null && file.getType().equals(typeFile);
    }

    public static class FileServiceException extends RuntimeException {
        public FileServiceException(String errorMsg) {
            super(errorMsg);
        }
    }

    public class FileAlreadyExists extends FileServiceException {
        public FileAlreadyExists(String filepath) {
            super("file " + filepath + " already exists");
        }
    }
    public static class FileDoesNotExist extends FileServiceException {
        public FileDoesNotExist(String filepath) {
            super("file " + filepath + " does not exist");
        }

        public FileDoesNotExist() {
            super("file does not exist");
        }
    }
    public class DirectoryWithTheSameName extends FileServiceException {
        public DirectoryWithTheSameName(String directoryPath) {
            super("directory " + directoryPath + " already exists");
        }
    }
    public class DirectoryDoesNotExist extends FileServiceException {
        public DirectoryDoesNotExist() { super("directory does not exist"); }
        public DirectoryDoesNotExist(String directoryPath) {
            super("directory " + directoryPath + " does not exist");
        }
    }
    public class NotInSwiftStorage extends FileServiceException {
        public NotInSwiftStorage(String id) {
            super(id + " not present in Swift storage");
        }
    }

    @Getter
    @AllArgsConstructor
    public static class NamedResource {
        private String filename;
        private Resource resource;
    }
}
