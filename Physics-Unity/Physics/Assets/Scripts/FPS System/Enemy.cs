using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.AI;

[RequireComponent(typeof(Ragdoll), typeof(NavMeshAgent))]
public class Enemy : MonoBehaviour
{
    public float health = 100;
    [SerializeField] private float damage = 10;
    [SerializeField, Tooltip("Degree/Second")] private float turnSpeed = 360;
    [SerializeField] private float attackRadius = 1;
    [SerializeField] private Transform playerCollider;

    public Collider HeadCollider => m_rd.animator.GetBoneTransform(HumanBodyBones.Head).GetComponent<Collider>();

    private Coroutine m_deathCR;
    private Ragdoll m_rd;
    private NavMeshAgent m_agent;
    private List<FPSController> m_players = new List<FPSController>();

    private Coroutine m_unRagdoll;

    private void Start()
    {
        m_rd = GetComponent<Ragdoll>();

        m_agent = GetComponent<NavMeshAgent>();
        m_agent.speed = 0;
        m_agent.angularSpeed = turnSpeed;
        m_agent.stoppingDistance = attackRadius;

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
            
            if (m_rd.TotalMovement < 1)
                m_deathCR ??= StartCoroutine(OnDeath().GetEnumerator());
            
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
        if (m_unRagdoll == null && m_rd.RagdollOn)
            m_unRagdoll = StartCoroutine(StopRagdoll().GetEnumerator());
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

    private IEnumerable OnDeath()
    {
        float waitTime = 3;
        yield return new WaitForSeconds(waitTime);
        
        m_rd.SetConstraints(RigidbodyConstraints.FreezeAll);
        
        float t = 0;
        float sinkTime = 5;
        Vector3 targetPos = transform.position + Vector3.down * 2;

        while(t < sinkTime)
        {
            transform.position = Vector3.Lerp(transform.position, targetPos, t / sinkTime);
            
            yield return null;
            
            t += Time.deltaTime;
        }

        Destroy(gameObject);
    }

    private IEnumerable StopRagdoll()
    {
        yield return new WaitForSeconds(3);

        m_rd.RagdollOn = false;
        m_unRagdoll = null;
    }
    
    private void SetTarget()
    {
        if(!m_agent.enabled || !m_agent.isOnNavMesh)
            return;

        if(!m_rd.RagdollOn)
        {
            GetClosestPlayer(out FPSController closestPlayer);
        
            if(closestPlayer != null)
            {
                m_agent.SetDestination(closestPlayer.transform.position);
            }
        }
        else m_agent.ResetPath();
    }

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

    private void GetClosestPlayer(out float _distance) => GetClosestPlayer(out FPSController closestPlayer, out _distance);

    private void GetClosestPlayer(out FPSController _closestPlayer) => GetClosestPlayer(out _closestPlayer, out float distance);
}
