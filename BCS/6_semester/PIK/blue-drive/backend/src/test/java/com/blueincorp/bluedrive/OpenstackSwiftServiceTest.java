package com.blueincorp.bluedrive;

import com.blueincorp.bluedrive.service.OpenstackSwiftService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;

import static org.junit.jupiter.api.Assertions.*;

@WebMvcTest(OpenstackSwiftService.class)
public class OpenstackSwiftServiceTest {
    @Autowired
    private OpenstackSwiftService swiftService;

    private final String TEST_CONTAINER = "test";

    @Test
    void uploadDownloadDelete() {
        var swiftGeneratedFilename = swiftService.uploadFile(new byte[]{Byte.parseByte("123")}, TEST_CONTAINER);
        var storedObject = swiftService.download(swiftGeneratedFilename, TEST_CONTAINER);
        assertNotNull(storedObject);
        assertTrue(swiftService.delete(swiftGeneratedFilename, TEST_CONTAINER));
    }

    @Test
    void getMetaFileNotInContainer() {
        assertTrue(swiftService.getMeta("not_generated_filename", TEST_CONTAINER).isEmpty());
    }

    // TODO change to assert throws
//    @Test
//    void downloadFileNotInContainer() {
//        assertTrue(swiftService.download("not_generated_filename", TEST_CONTAINER).isEmpty());
//    }

    @Test
    void deleteFileNotInContainer() {
        assertFalse(swiftService.delete("not_generated_filename", TEST_CONTAINER));
    }

}
