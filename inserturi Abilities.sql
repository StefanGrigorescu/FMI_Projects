--delete from abilities;

insert into abilities
values('Defender', 1, 0, 
'You can alert this unit to stop an attack of an enemy unit. Then, both units fight.');

insert into abilities
values('Mad raider', 4, 0,
'During its attacks, this unit has increased power.');

insert into abilities
values('Counter-offensive', 7, 0, 
'When this unit returns from a destroyed stronghold, you may deploy it immediately on the battlefield, for no cost, fighting the unit that stormed the stronghold.');

insert into abilities
values('Ancient curse', 7, 0, 
'When this rune is recovered from a destroyed stronghold, you may invoke it immediately, for no cost.');

insert into abilities
values('Resource gatherer', 3, 0,
'When this unit joins the battlefield, add the specified amount of top cards of your deck to your energy supplies.');

insert into abilities
values('Reaper warrior', 4, 1,
'Whenever an attack of this unit encounters no defense, it kills the weakest enemy unit (you decide in case of a tie).');

insert into abilities
values('Scout', 3, 0,
'When this unit joins the battlefield, you may alert an enemy unit.');

insert into abilities
values('Defender bane', 0, 0,
'Kill one of the enemy defenders.');

insert into abilities
values('Cavalry assault', 3, 0,
'When this creature joins the battlefield, it may attack on the same turn.');

insert into abilities
values('Barehanded', 0, 0,
'This unit can not storm strongholds.');

insert into abilities
values('Safe hit', 0, 0,
'Kill one of the enemy units that has a power level less than or equal to the specified amount.');

insert into abilities
values('Death and taxes', 0, 0,
'Kill one of the enemy units.');

select * from abilities;




