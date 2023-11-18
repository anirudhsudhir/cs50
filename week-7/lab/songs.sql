-- 1
select name
from songs
;

-- 2
select name
from songs
order by tempo
;

-- 3
select name
from songs
order by duration_ms desc
limit 5
;

-- 4
select name
from songs
where danceability > 0.75 and energy > 0.75 and valence > 0.75
;

-- 5
select avg(energy)
from songs
;

-- 6
select name
from songs
where artist_id in (select id from artists where name = 'Post Malone')
;

-- 7
select avg(energy)
from songs
where artist_id = (select id from artists where name = 'Drake')
;

-- 8
select name
from songs
where name like '%feat.%'
;
