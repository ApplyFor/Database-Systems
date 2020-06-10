SELECT (walkDistance+swimDistance+rideDistance) AS totalDistance, walkDistance, swimDistance, rideDistance
FROM player_statistic
WHERE walkDistance!=0 AND swimDistance!=0 AND rideDistance!=0
ORDER BY totalDistance DESC
LIMIT 10