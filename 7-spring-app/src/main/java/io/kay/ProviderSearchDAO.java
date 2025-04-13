package io.kay;

import jakarta.persistence.EntityManager;
import jakarta.persistence.Query;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class ProviderSearchDAO {

    @Autowired
    private EntityManager em;

    public List<Object[]> getProvidersInZipCode(String zipCode) {
        String q = "select * from PROVIDERS where public_listing is true and zip_code = :zipCode";
        Query query = em.createNativeQuery(q);
        query.setParameter("zipCode", zipCode);
        return query.getResultList();
    }
}
