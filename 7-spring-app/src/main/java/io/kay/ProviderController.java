package io.kay;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

@RestController
public class ProviderController {

    @Autowired
    ProviderSearchDAO providerSearchDAO;

    private int someNumber = 0;

    @GetMapping("/search")
    public List<Object[]> search(@RequestParam("zipCode") String zipCode) throws ClassNotFoundException {
        System.out.println(Arrays.toString(Class.forName("io.kay.Main").getConstructors()));
        return providerSearchDAO.getProvidersInZipCode(zipCode);
    }

    @GetMapping("/getNumber")
    public int getNumber() {
        return someNumber;
    }

    @PutMapping("/modifyNumber")
    public int modify(@RequestParam("number") int number) {
        someNumber = number;
        return someNumber;
    }
}
