--delete from AspNetRoles;

insert into AspNetRoles
values('Admin', 'Admin', 'ADMIN', null);

insert into AspNetRoles
values('BasicUser', 'BasicUser', 'BASICUSER', null);

select * from AspNetRoles;

select * from AspNetUsers;

select * from AspNetUserRoles;