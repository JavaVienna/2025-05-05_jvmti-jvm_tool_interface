package io.kay;

import jakarta.ws.rs.*;

@Path("/")
public class ProviderController {

    private int someNumber = 0;

    @GET
    @Path("/getNumber")
    public int getNumber() {
        return someNumber;
    }

    @PUT
    @Path("/modifyNumber")
    public int modify(@QueryParam("number") int number) {
        someNumber = number;
        return someNumber;
    }
}
