using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RaycastImpulse : MonoBehaviour
{
    [SerializeField] private float hitForce = 500;

    // Start is called before the first frame update
    private void Start()
    {
        
    }

    // Update is called once per frame
    private void Update()
    {
        if(Input.GetMouseButton(0))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            RaycastHit hitPoint;

            if(Physics.Raycast(ray, out hitPoint, 500, -1, QueryTriggerInteraction.Ignore))
            {
                Ragdoll ragdoll = hitPoint.collider.GetComponentInParent<Ragdoll>();

                if(ragdoll != null)
                    ragdoll.ragdollOn = true;

                Rigidbody rb = hitPoint.collider.GetComponent<Rigidbody>();

                if(rb != null)
                    rb.AddForce(ray.direction * hitForce);
            }
        }
    }
}
