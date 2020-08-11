 -- insert
INSERT INTO Target (ID, Value)
SELECT ID, Value FROM Source
WHERE NOT EXISTS (SELECT * FROM Target WHERE Target.ID = Source.ID);

-- delete
DELETE FROM Target
WHERE NOT EXISTS (SELECT * FROM Source WHERE Target.ID = Source.ID)

-- update
UPDATE Target
JOIN Source ON Target.ID = Source.ID
SET Value = Source.Value
