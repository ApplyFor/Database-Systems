SELECT matchType, COUNT(*) count
FROM `match`
WHERE matchType LIKE '%fpp'
GROUP BY matchType
ORDER BY COUNT(*);