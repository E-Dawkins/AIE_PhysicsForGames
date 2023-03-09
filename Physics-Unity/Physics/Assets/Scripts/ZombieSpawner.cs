using System;
using System.Collections.Generic;

using UnityEngine;

using Random = UnityEngine.Random;

public class ZombieSpawner : MonoBehaviour
{
	[SerializeField] private Vector2 maxXZ;
	[SerializeField] private Vector2 minXZ;

	[SerializeField] private Zombie zombiePrefab;
	[SerializeField] private List<Zombie> spawnedZombies = new List<Zombie>();

	[SerializeField] private int zombiesInWave = 10;

	private void Update()
	{
		// Spawn new wave of zombies
		if(spawnedZombies.Count == 0)
			SpawnZombies();

		// Remove any deleted zombies, i.e. they dead
		for(int i = spawnedZombies.Count - 1; i >= 0; i--)
		{
			if (spawnedZombies[i] == null)
				spawnedZombies.RemoveAt(i);
		}
	}

	private void SpawnZombies()
	{
		for(int i = 0; i < zombiesInWave; i++)
		{
			float randX = Random.Range(minXZ.x, maxXZ.x);
			float randZ = Random.Range(minXZ.y, maxXZ.y);

			Vector3 zombiePos = new Vector3(randX, 0, randZ);

			Zombie zombie = Instantiate(zombiePrefab, zombiePos, Quaternion.identity, transform);
			
			spawnedZombies.Add(zombie);
		}
	}
}