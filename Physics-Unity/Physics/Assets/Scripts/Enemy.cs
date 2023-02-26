using UnityEngine;

public class Enemy : MonoBehaviour
{
    public float health = 100;

    // Start is called before the first frame update
    private void Start()
    {
        
    }

    // Update is called once per frame
    private void Update()
    {
        
    }

    public void DoDamage(float _damage)
    {
        health -= _damage;
        
        // Spawn hit effect
        Debug.Log($"{health}");
        
        // If dead, run death function
        if (health <= 0)
            OnDeath();
    }

    private void OnDeath()
    {
        
    }
}
