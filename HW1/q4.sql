SELECT Id, AVG(kills) avgKills
FROM (SELECT matchId, numGroups
		FROM `match`
		WHERE numGroups<=10) AS `match`
	INNER JOIN (SELECT Id, matchId, kills
					FROM player_statistic) AS player_statistic
		USING (matchId)
GROUP BY Id
ORDER BY avgKills DESC
LIMIT 20;