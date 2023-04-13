using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.AI;

[RequireComponent(typeof(Ragdoll), typeof(NavMeshAgent))]
public class Zombie : MonoBehaviour
{
    public float health = 100;
    [SerializeField] private float damage = 10;
    [SerializeField, Tooltip("Degree/Second")] private float turnSpeed = 360;
    [SerializeField] private float attackRadius = 1;
    [SerializeField] private Transform playerCollider;
    
    public Collider HeadCollider => m_rd.animator.GetBoneTransform(HumanBodyBones.Head).GetComponent<Collider>();

    public Ragdoll Ragdoll => m_rd;
    private Ragdoll m_rd;
    
    public NavMeshAgent Agent => m_agent;
    private NavMeshAgent m_agent;
    
    private List<FPSController> m_players = new List<FPSController>();

    private Coroutine m_unRagdoll_CR;
    private Coroutine m_death_CR;

    private void Start()
    {
        m_rd = GetComponent<Ragdoll>();

        m_agent = GetComponent<NavMeshAgent>();
        m_agent.speed = 0;
        m_agent.angularSpeed = turnSpeed;
        m_agent.stoppingDistance = attackRadius;

        // Add each player to a list of player controllers
        foreach(FPSController player in FindObjectsOfType<FPSController>())
        {
            m_players.Add(player);
        }
    }
    
    private void Update()
    {
        // Agent is off if ragdolling
        m_agent.enabled = !m_rd.RagdollOn;
        m_agent.speed = m_rd.animator.GetFloat("ZombieSpeed");
        
        // Set target to be nearest player, if not ragdolling
        SetTarget();

        // If dead, run death coroutine
        if(health <= 0)
        {
            m_rd.RagdollOn = true;

            m_death_CR ??= StartCoroutine(OnDeath().GetEnumerator());

            return;
        }
        
        // Move the player collider, only for walk type 3 (crawl-run)
        if(m_rd.animator.GetInteger("WalkType") == 3)
        {
            playerCollider.transform.localPosition = new Vector3(0, 0.25f, 0);
            playerCollider.transform.localEulerAngles = new Vector3(90, 0, 0);
        }

        AttackLogic();

        // Run un-ragdoll coroutine, only if not already running
        if (m_unRagdoll_CR == null && m_rd.RagdollOn)
            m_unRagdoll_CR = StartCoroutine(UnRagdoll().GetEnumerator());
    }

    private void AttackLogic()
    {
        GetClosestPlayer(out FPSController closestPlayer, out float distance);
        
        // Player is in attack radius, start the attack
        if(distance <= attackRadius)
        {
            // Set start attack to true
            if (!m_rd.animator.GetBool("StartAttack"))
                m_rd.animator.SetBool("StartAttack", true);
        }
        
        // End of attack triggered
        if(m_rd.animator.GetBool("EndAttack"))
        {
            if (distance <= attackRadius) // player within radius, damage them
                closestPlayer.health -= damage;
            
            m_rd.animator.SetBool("EndAttack", false);
        }
    }

    private IEnumerable UnRagdoll()
    {
        // Wait for delay
        yield return new WaitForSeconds(3);

        // Then wait until total movement is low enough
        while(m_rd.TotalMovement() > 1)
            yield return null;

        // Then un-ragdoll
        m_rd.RagdollOn = false;
        m_unRagdoll_CR = null;
    }

    private IEnumerable OnDeath()
    {
        // Wait for delay
        yield return new WaitForSeconds(3);

        // Then destroy the game object
        Destroy(gameObject);
    }

    private void SetTarget() // set nav mesh agent target to nearest player
    {
        if(!m_agent.enabled || !m_agent.isOnNavMesh)
            return;

        // Only if not ragdolling
        if(!m_rd.RagdollOn)
        {
            GetClosestPlayer(out FPSController closestPlayer);
        
            if(closestPlayer != null)
            {
                m_agent.SetDestination(closestPlayer.transform.position);
            }
        }
        
        // If ragdolling, reset agent path
        else m_agent.ResetPath();
    }

    // Helper function to get closest player, outputting the closest player and distance to player
    private void GetClosestPlayer(out FPSController _closestPlayer, out float _distance)
    {
        _distance = float.MaxValue;
        _closestPlayer = null;

        foreach(FPSController player in m_players)
        {
            float playerDist = Vector3.Distance(player.transform.position, transform.position);

            if(playerDist < _distance)
            {
                _closestPlayer = player;
                _distance = playerDist;
            }
        }
    }

    // Same function but only to get the distance
    private void GetClosestPlayer(out float _distance) => GetClosestPlayer(out FPSController closestPlayer, out _distance);

    // Same function but only to get the player
    private void GetClosestPlayer(out FPSController _closestPlayer) => GetClosestPlayer(out _closestPlayer, out float distance);
}
