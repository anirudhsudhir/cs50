-- 1
select title
from movies
where year = 2008
;

-- 2
select birth
from people
where name = 'Emma Stone'
;

-- 3
select title
from movies
where year >= 2018
order by title
;

-- 4
select count(*)
from ratings
where rating = 10
;

-- 5
select title, year
from movies
where title like 'Harry Potter%'
order by year
;

-- 6
select avg(rating)
from ratings
where movie_id in (select id from movies where year = 2012)
;

-- 7
select movies.title, ratings.rating
from movies
join ratings on movies.id = ratings.movie_id
where movies.year = 2010
order by ratings.rating desc, movies.title
;

-- 8
select name
from people
where
    id in (
        select person_id
        from stars
        where movie_id = (select id from movies where title = 'Toy Story')
    )
;

-- 9
select distinct name
from people
where
    id in (
        select person_id
        from stars
        where movie_id in (select id from movies where year = 2004)
    )
order by birth
;

-- 10
select name
from people
where
    id in (
        select person_id
        from directors
        where movie_id in (select movie_id from ratings where rating >= 9.0)
    )
group by name
;

-- 11
select title
from movies, ratings
where
    ratings.movie_id = movies.id
    and ratings.movie_id in (
        select movie_id
        from stars
        where person_id = (select id from people where name = 'Chadwick Boseman')
    )
order by ratings.rating desc
limit 5
;

-- 12
select title
from movies
where
    id in (
        select movie_id
        from stars
        where person_id = (select id from people where name = 'Bradley Cooper')
        intersect
        select movie_id
        from stars
        where person_id = (select id from people where name = 'Jennifer Lawrence')
    )
;

-- 13
select name
from people
where
    id in (
        select person_id
        from stars
        where
            movie_id in (
                select movie_id
                from stars
                where
                    person_id = (
                        select id
                        from people
                        where name = 'Kevin Bacon' and birth = 1958
                    )
            )
            and person_id
            != (select id from people where name = 'Kevin Bacon' and birth = 1958)
    )
;
