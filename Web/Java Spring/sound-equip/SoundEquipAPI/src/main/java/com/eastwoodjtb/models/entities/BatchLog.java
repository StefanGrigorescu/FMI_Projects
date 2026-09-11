package com.eastwoodjtb.models.entities;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.util.Date;


@Entity
@Table(name="batch_log")
@Data
@EqualsAndHashCode//(callSuper = false)
@NoArgsConstructor
public class BatchLog {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "batch_log_id")
    private Long id;

    @Column(name="log_date")
    private Date logDate;

    @Column(name="editor_username")
    private String editorUsername;

}
