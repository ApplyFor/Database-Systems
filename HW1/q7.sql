SELECT COUNT(winPlacePerc) numberOfPlayers, MAX(heals) maxHeals
FROM player_statistic
	INNER JOIN (SELECT matchId, matchDuration
					FROM `match`
					WHERE matchDuration>(SELECT AVG(matchDuration)
												FROM `match`)) AS `match`
		USING (matchId)
WHERE damageDealt=0 AND winPlacePerc=1