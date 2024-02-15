using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using Unity.VisualScripting;
using UnityEditor.ShaderGraph.Serialization;
using Newtonsoft.Json;
using System.Dynamic;
using static UnityEditor.PlayerSettings;
using static UnityEngine.Rendering.DebugUI.Table;
using UnityEngine.UIElements;
using UnityEngine.Tilemaps;
using UnityEngine.Rendering;
using static PlasticGui.PlasticTableColumn;

public class MenuItems
{
    [MenuItem("Tools/Export all #_x")]
    private static void NewMenuOption()
    {
        string path = EditorUtility.SaveFilePanel("Export scene...", "../", "exportedLevelASCII.txt", "txt");

        if (path.Length != 0)
        {
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(path))
            {
                GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>();
                GameObject[] rootObjects = UnityEngine.SceneManagement.SceneManager.GetActiveScene().GetRootGameObjects();

                dynamic export = new ExpandoObject();
                export.project_root = Application.productName;
                export.scene_objects = new List<dynamic>(rootObjects.Length);

                foreach (GameObject go in rootObjects)
                {
                    dynamic obj = Traverse(go);

                    export.scene_objects.Add(obj);
                }

                string json = JsonConvert.SerializeObject(export);
                file.WriteLine(json);
            }

        }
    }
    private static dynamic Traverse(GameObject go)
    {
        dynamic obj = new ExpandoObject();

        //obj.Tag = go.tag;
        obj.Name = go.name;

        List<dynamic> components = new List<dynamic>();
        obj.Components = components;
        //Quaternion rot = go.transform.rotation;

        // we use anonymous object, they work similar to json in many ways, and is simple to serialize to json! just what we need ^^

        {
            dynamic tag = new ExpandoObject();
            tag.Type = "Tag";
            tag.Data = new { Tag = go.tag };
            components.Add(tag);
        }

        { // You can more or less put any data you want as long as it keeps serializable (i.e. can be converted to text, or json in our case)
            //obj.rotation = new { x = rot.x, y = rot.y, z = rot.z };

            dynamic transform = new ExpandoObject();
            transform.Type = "Transform";
            {
                dynamic data = new ExpandoObject();

                Vector3 rot = go.transform.rotation.eulerAngles;
                Vector3 pos = go.transform.localPosition;
                Vector3 scale = go.transform.localScale;

                data.Scale = new float[3] { scale.x, scale.y, scale.z };
                data.Translation = new float[3] { pos.x * 32, pos.y * 32, pos.z * 32 };
                data.Rotation = new float[3] { Mathf.Deg2Rad * rot.x, Mathf.Deg2Rad * rot.y, Mathf.Deg2Rad * rot.z };
                transform.Data = data;
            }

            components.Add(transform);
        }
        // Above is common for all things, they have name and transform

        // Some special cases, for example the Camera we want information about field of view and the near and far clipping plane
        { // since we are using a dynamic object we can simply add the information if we happen to find a camera component!
            //var component = go.GetComponent<Camera>();
            //if (component)
            //{
            //    obj.type = "camera";    // object type is used in TGE to identify what data we can assume each object to have

            //    // Unity fov is always vertical, we need it horizontal!
            //    obj.fov = Camera.VerticalToHorizontalFieldOfView(component.fieldOfView, component.aspect);
            //    obj.far = component.farClipPlane;
            //    obj.near = component.nearClipPlane;
            //}
        }
        { // For light sources you might want some information, [[-- this is left for your to figure out :) --]]
            //var component = go.GetComponent<Light>();
            //if (component)
            //{
            //    obj.type = "light";
            //}
        }
        { // Sprites are handled different from 3D-models
            var component = go.GetComponent<SpriteRenderer>();
            if (component)
            {
                dynamic spriteRenderer = new ExpandoObject();
                spriteRenderer.Type = "SpriteRenderer2D";
                {
                    dynamic data = new ExpandoObject();

                    data.Texture = /*Application.productName + "/" + */AssetDatabase.GetAssetPath(component.sprite);
                    data.Size = new float[2] {
                        component.sprite.texture.width,
                        component.sprite.texture.height
                    };
                    data.Pivot = new float[2] {
                        component.sprite.pivot.x / component.sprite.texture.width,
                        1.0f - component.sprite.pivot.y / component.sprite.texture.height
                    };
                    data.Color = new float[4] {
                        component.color.r,
                        component.color.g,
                        component.color.b,
                        component.color.a
                    };

                    data.OrderInLayer = component.sortingOrder;

                    spriteRenderer.Data = data;
                }
                components.Add(spriteRenderer);

                //obj.type = "sprite";

                //obj.pivot = new
                //{
                //    x = component.sprite.pivot.x / component.sprite.texture.width,
                //    y = component.sprite.pivot.y / component.sprite.texture.height
                //};

                //obj.path = Application.productName + "/" + AssetDatabase.GetAssetPath(component.sprite);
            }
        }
        //{ // Models has some special data
        //    var component = go.GetComponent<MeshFilter>();
        //    if (component)
        //    {
        //        obj.type = "model";
        //        obj.path = Application.productName + "/" + AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(component));
        //    }
        //}
        //{ // And to find the correct assets for Animated models we need to do like this
        //    var component = go.GetComponent<Animator>();
        //    if (component)
        //    {
        //        obj.type = "animated_model";
        //        obj.path = Application.productName + "/" + AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(component));
        //    }
        //}

        {
            var component = go.GetComponent<Tilemap>();
            var renderer = go.GetComponent<TilemapRenderer>();
            if (component && renderer)
            {
                dynamic tilemap = new ExpandoObject();
                tilemap.Type = "Tilemap";
                {
                    dynamic data = new ExpandoObject();
                    data.OrderInLayer = renderer.sortingOrder;
                    data.Sprites = new List<string>();
                    data.TileSprites = new List<TileSpriteInfo>();
                    data.Tiles = new List<int[]>();

                    for (int x = component.cellBounds.min.x; x < component.cellBounds.max.x; x++)
                    {
                        for (int y = component.cellBounds.min.y; y < component.cellBounds.max.y; y++)
                        {
                            for (int z = component.cellBounds.min.z; z < component.cellBounds.max.z; z++)
                            {
                                Vector3Int tilePos = new Vector3Int(x, y, z);
                                if (component.HasTile(tilePos))
                                {
                                    int[] tile = new int[3];

                                    tile[0] = x;
                                    tile[1] = y;

                                    Sprite tileSprite = component.GetSprite(tilePos);
                                    string spritePath = AssetDatabase.GetAssetPath(tileSprite);
                                    int pathIndex = data.Sprites.IndexOf(spritePath);
                                    if (pathIndex == -1)
                                    {
                                        pathIndex = data.Sprites.Count;
                                        data.Sprites.Add(spritePath);
                                    }

                                    TileSpriteInfo info;
                                    info.SpriteIndex = pathIndex;
                                    info.MinX = tileSprite.textureRect.min.x / tileSprite.texture.width;
                                    info.MinY = 1.0f - tileSprite.textureRect.max.y / tileSprite.texture.height;
                                    info.MaxX = tileSprite.textureRect.max.x / tileSprite.texture.width;
                                    info.MaxY = 1.0f - tileSprite.textureRect.min.y / tileSprite.texture.height;

                                    int spriteInfoIndex = data.TileSprites.IndexOf(info);
                                    if (spriteInfoIndex == -1)
                                    {
                                        spriteInfoIndex = data.TileSprites.Count;
                                        data.TileSprites.Add(info);
                                    }
                                    tile[2] = spriteInfoIndex;

                                    data.Tiles.Add(tile);
                                }
                            }
                        }
                    }
                    tilemap.Data = data;
                }
                components.Add(tilemap);
            }
        }

        if (go.GetComponent<TilemapCollider2D>())
        {
            dynamic tilemapCollider = new ExpandoObject();
            tilemapCollider.Type = "TilemapColliderGenerator";
            tilemapCollider.Data = new object();

            components.Add(tilemapCollider);
        }

        obj.Children = new List<dynamic>();
        foreach (Transform child in go.transform)
        {
            obj.Children.Add(Traverse(child.gameObject));
        }

        return obj;
    }
}
