package com.blueincorp.bluedrive;

import com.blueincorp.bluedrive.controllers.FileController;
import com.blueincorp.bluedrive.requestbody.FileInfo;
import com.blueincorp.bluedrive.service.FileService;
import com.blueincorp.bluedrive.service.OpenstackSwiftService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.context.annotation.Import;
import org.springframework.core.io.ByteArrayResource;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.util.Arrays;
import java.util.Optional;

import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;


@WebMvcTest(FileController.class)
@Import({OpenstackSwiftService.class})
public class FileControllerTest {
    @MockBean
    private FileService fileService;
    @MockBean
    private OpenstackSwiftService openstackSwiftService;


    private FileController fileController;

    private MockMvc mvc;

    private final String URL_PREFIX = "/api";
    private final String TEST_USER = "test";

    // https://stackoverflow.com/questions/25335343/spring-junit-mockito-unit-test-controller-specified-class-is-an-interface
    @BeforeEach
    public void setUp() {
        fileController = new FileController(openstackSwiftService, fileService);
        mvc = MockMvcBuilders
                .standaloneSetup(fileController)
                .build();
    }

    @Test
    @WithMockUser(TEST_USER)
    void upload() throws Exception {
        MockMultipartFile mockMultipartFile = new MockMultipartFile(
                "file",
                "filename.txt",
                "multipart/form-data",
                "file_data".getBytes());
        mvc.perform(MockMvcRequestBuilders.multipart(URL_PREFIX + "/file?path=/A/filename.txt")
                .file(mockMultipartFile))
                .andExpect(status().isCreated())
                .andReturn();
    }

    @Test
    @WithMockUser(TEST_USER)
    void getMetadataID() throws Exception {
        when(fileService.getMetadataByID("123", TEST_USER)).thenReturn(
                FileInfo.builder()
                .id("123")
                .filename("test_file.txt")
                .directoryPath("/A/B")
                .build()
        );
        when(openstackSwiftService.getMeta("123", TEST_USER)).thenReturn(
                Optional.of(FileInfo.builder()
                    .id("123")
                    .build()
                )
        );
        mvc.perform(MockMvcRequestBuilders.get(URL_PREFIX + "/meta/123"))
                .andExpect(status().isOk());
    }

    @Test
    @WithMockUser(TEST_USER)
    void getMetadataByIDFileNotInContainerExpect204() throws Exception {
        final String ID_NOT_IN_CONTAINER = "not_in_container";
        when(fileService.getMetadataByID(ID_NOT_IN_CONTAINER, TEST_USER)).thenThrow(new FileService.FileDoesNotExist());
        mvc.perform(MockMvcRequestBuilders.get(URL_PREFIX + "/meta/" + ID_NOT_IN_CONTAINER))
                .andExpect(status().isConflict());
    }

    @Test
    @WithMockUser(TEST_USER)
    void download() throws Exception {
        when(fileService.downloadFile("123", TEST_USER)).thenReturn(
                new FileService.NamedResource(
                        "test.txt",
                        new ByteArrayResource("resource_data".getBytes())));
        mvc.perform(MockMvcRequestBuilders.get(URL_PREFIX + "/file/123"))
                .andExpect(status().isOk());
    }

    @Test
    @WithMockUser(TEST_USER)
    void deleteFileNotInContainerExpect204() throws Exception {
        final String ID_NOT_IN_CONTAINER = "not_in_container";
        when(fileService.downloadFile(ID_NOT_IN_CONTAINER, TEST_USER)).thenThrow(
                new FileService.FileDoesNotExist()
        );
        mvc.perform(MockMvcRequestBuilders.get(URL_PREFIX + "/file/not_in_container"))
                .andExpect(status().isConflict());
    }

    @Test
    @WithMockUser(TEST_USER)
    void listDir() throws Exception {
        final String TEST_DIR = "/test_dir";
        when(fileService.listDirectory(TEST_DIR, TEST_USER)).thenReturn(
            Arrays.asList(new FileInfo[]{
                    FileInfo.builder()
                            .id("123")
                            .filename("test_file.txt")
                            .directoryPath("/A/B")
                            .build()
            })
        );
        mvc.perform(MockMvcRequestBuilders.get(URL_PREFIX + "/directory?path=" + TEST_DIR))
                .andExpect(status().isOk())
                .andExpect(content().json("[{\"filename\":\"test_file.txt\",\"directoryPath\":\"/A/B\",\"id\":\"123\"," +
                        "\"lastModified\":null,\"size\":null,\"contentType\":null}]"))
                .andReturn();
    }

}
