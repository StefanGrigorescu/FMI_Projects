package com.eastwoodjtb.models.entities;

import com.eastwoodjtb.models.Model;
import com.fasterxml.jackson.annotation.JsonBackReference;
import lombok.*;

import javax.persistence.*;
import java.util.Date;


@Entity
@Table(name="product")
@Data
@EqualsAndHashCode(callSuper = false)
@NoArgsConstructor
public class Product implements Model {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "product_id")
    private Long id;
    @Column(name = "name")
    private String name;
    @Column(name = "status")
    private String status;
    @Column(name = "quantity")
    private int quantity;
    @Column(name="stock_decl_date")
    private Date stockDeclDate;

    @ManyToOne
    @JoinColumn(name="subsidiary_id")
    @JsonBackReference
    private Subsidiary subsidiary;


    @Override
    public Long getId() {
        return id;
    }

    @Override
    public String getName() {
        return name;
    }

}
