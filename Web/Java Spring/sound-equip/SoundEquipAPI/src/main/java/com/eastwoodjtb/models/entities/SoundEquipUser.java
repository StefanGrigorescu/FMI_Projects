package com.eastwoodjtb.models.entities;


import com.eastwoodjtb.models.Model;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.Collection;


@Entity
@Table(name="sound_equip_user")
@Data
@EqualsAndHashCode(callSuper = false)
@NoArgsConstructor
public class SoundEquipUser implements Model {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "sound_equip_user_id")
    private Long id;

    @Column(name = "name")
    private String name;

    @Column(name = "password")
    private String password;

    @Column(name = "ftp_username")
    private String ftpUsername;

    @Column(name = "ftp_password")
    private String ftpPassword;

    @Column(name = "ftp_logout_state")
    private boolean ftpLogoutState;

    @Column(name = "access_token")
    private String accessToken;

    @Column(name = "refresh_token")
    private String refreshToken;

    @ManyToMany(fetch = FetchType.EAGER)
    private Collection<Role> roles = new ArrayList<>();


    public Long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

}
