package com.eastwoodjtb.models.entities;

import com.eastwoodjtb.models.Model;
import com.fasterxml.jackson.annotation.JsonManagedReference;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.util.List;


@Entity
@Table(name="subsidiary")
@Data
@EqualsAndHashCode(callSuper = false)
@NoArgsConstructor
public class Subsidiary implements Model {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "subsidiary_id")
    private Long id;
    @Column(name = "name")
    private String name;
    @Column(name="country_code")
    private String countryCode;
    @OneToMany(mappedBy = "subsidiary")
    @JsonManagedReference
    private List<Product> products;



    @Override
    public Long getId() {
        return id;
    }

    @Override
    public String getName() {
        return name;
    }

}
