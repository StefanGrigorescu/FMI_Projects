package com.eastwoodjtb.security;

import com.eastwoodjtb.models.entities.Role;
import com.eastwoodjtb.models.entities.SoundEquipUser;
import com.eastwoodjtb.services.RoleService;
import com.eastwoodjtb.services.SoundEquipUserService;
import lombok.RequiredArgsConstructor;
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;


@Component
@RequiredArgsConstructor
public class InitialSeed implements CommandLineRunner {
    private static final String admin1Name;
    private static final String admin1Password;

    private final SoundEquipUserService soundEquipUserService;
    private final RoleService roleService;


    static
    {
        admin1Name = "SoundEquipAdmin";
        admin1Password = "eastwoodjtbP@s5word";

    }


    @Override
    public void run(String... args) throws Exception {
        loadRolesAndUsers();
    }

    private void loadRolesAndUsers()
    {
        if(roleService.getOneByName(RoleEnum.Admin.getName()) == null)
        {
            Role adminRole = new Role();
            adminRole.setName(RoleEnum.Admin.getName());
            roleService.create(adminRole);
        }
        if(roleService.getOneByName(RoleEnum.Manager.getName()) == null)
        {
            Role managerRole = new Role();
            managerRole.setName(RoleEnum.Manager.getName());
            roleService.create(managerRole);
        }
        if(roleService.getOneByName(RoleEnum.BasicUser.getName()) == null)
        {
            Role basicUserRole = new Role();
            basicUserRole.setName(RoleEnum.BasicUser.getName());
            roleService.create(basicUserRole);
        }
        if(soundEquipUserService.getOneByName(admin1Name) == null)
        {
            SoundEquipUser admin1 = new SoundEquipUser();
            admin1.setName(admin1Name);
            admin1.setPassword(admin1Password);
            admin1.setFtpLogoutState(false);
            admin1.setRefreshToken(null);
            admin1.setRefreshToken(null);
            soundEquipUserService.create(admin1);

            soundEquipUserService.addRole(admin1Name, RoleEnum.Admin.getName());
            soundEquipUserService.addRole(admin1Name, RoleEnum.Manager.getName());
            soundEquipUserService.addRole(admin1Name, RoleEnum.BasicUser.getName());
        }
    }

}
