SELECT teamRoadKills, AVG(tmp_avgWinPlacePerc) avgWinPlacePerc
FROM (SELECT  SUM(roadKills) teamRoadKills, AVG(winPlacePerc) tmp_avgWinPlacePerc
		FROM `match` M, player_statistic P
		WHERE  M.matchId=P.matchId AND (matchType LIKE 'squad' OR matchType LIKE 'squad-fpp')
		GROUP BY groupId) AS tmp
GROUP BY teamRoadKills
ORDER BY teamRoadKills DESC;