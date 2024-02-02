package com.blueincorp.bluedrive.controllers;

import com.blueincorp.bluedrive.requestbody.FileInfo;
import com.blueincorp.bluedrive.service.FileService;
import com.blueincorp.bluedrive.service.OpenstackSwiftService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.util.List;

@RestController
@RequestMapping("api")
public class FileController {
    private Logger logger = LoggerFactory.getLogger(FileController.class);
    private final OpenstackSwiftService swiftService;
    private final FileService fileService;

    @Autowired
    public FileController(OpenstackSwiftService swiftService, FileService fileService) {
        this.swiftService = swiftService;
        this.fileService = fileService;
    }

    @PostMapping("/file")
    public ResponseEntity<String> upload(@RequestParam(name = "file") MultipartFile file, @RequestParam String path) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            fileService.createFile(path, containerName, file.getBytes());
            return new ResponseEntity("", HttpStatus.CREATED);
        }
        catch (IOException e) {
            return new ResponseEntity("", HttpStatus.BAD_REQUEST);
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        }
    }

    @PostMapping("/directory")
    public ResponseEntity<String> createDirectory(@RequestParam String path) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            fileService.createDirectory(path, containerName);
            return new ResponseEntity("", HttpStatus.CREATED);
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        }
    }

    @GetMapping("/meta/{id}")
    public ResponseEntity<FileInfo> getMetadataByID(@PathVariable("id") String id) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            var metadata = fileService.getMetadataByID(id, containerName);
            return new ResponseEntity(metadata, HttpStatus.OK);
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        }
    }

    @DeleteMapping("/file/debug/delete-all")
    public ResponseEntity deleteAll() {
        fileService.deleteAll();
        swiftService.deleteAll();
        return new ResponseEntity("", HttpStatus.OK);
    }

    @GetMapping("/directory")
    public ResponseEntity<List<FileInfo>> listDir(@RequestParam String path) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        return new ResponseEntity(fileService.listDirectory(path, containerName), HttpStatus.OK);
    }

    @GetMapping("/file/{filename}")
    public ResponseEntity<Resource> download(@PathVariable("filename") String id) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            var namedResource = fileService.downloadFile(id, containerName);
            return ResponseEntity
                    .ok()
                    .contentType(MediaType.APPLICATION_OCTET_STREAM)
                    .contentLength(namedResource.getResource().contentLength())
                    .header(
                            "Content-Disposition",
                            "attachment;filename=\"" + namedResource.getFilename() + "\"")
                    .body(namedResource.getResource());
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        } catch (IOException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }

    @DeleteMapping("/file/{id}")
    public ResponseEntity<String> delete(@PathVariable("id") String id) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            fileService.deleteFile(id, containerName);
            return new ResponseEntity<>("", HttpStatus.OK);
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        }
    }

    @DeleteMapping("/directory/{id}")
    public ResponseEntity<String> deleteDirectory(@PathVariable("id") String id) {
        String containerName = SecurityContextHolder.getContext().getAuthentication().getName();
        try {
            fileService.deleteDirectory(id, containerName);
            return new ResponseEntity<>("", HttpStatus.OK);
        }
        catch (FileService.FileServiceException e) {
            return new ResponseEntity(e.getMessage(), HttpStatus.CONFLICT);
        }
    }
}
