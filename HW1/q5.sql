SELECT matchType, AVG(matchDuration) averageDuration
FROM `match`
GROUP BY matchType
ORDER BY averageDuration