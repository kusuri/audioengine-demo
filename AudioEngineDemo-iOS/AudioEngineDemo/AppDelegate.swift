//
//  AppDelegate.swift
//  AudioEngineDemo
//
//  Created by Adam Wilson on 06/11/2018.
//  Copyright © 2018 YourCompany. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        
        window = UIWindow(frame: UIScreen.main.bounds)

        let mainStoryboard: UIStoryboard = UIStoryboard(name: "Main", bundle: nil)
        let demoViewController: ViewController = mainStoryboard.instantiateViewController(withIdentifier: "DemoController") as! ViewController
        
        self.window?.rootViewController = demoViewController

        window?.makeKeyAndVisible()

        // Copy default files to app Documents folder
        // (There is probably a cleaner way to do this!)
        let file1URL = Bundle.main.url(forResource: "Chhhhaah", withExtension: ".wav")
        let file2URL = Bundle.main.url(forResource: "The VERONA Break edit", withExtension: ".wav")
        
        if let documentsPathURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first {
            let fullDest1URL = documentsPathURL.appendingPathComponent("Chhhhaah.wav")
            let fullDest2URL = documentsPathURL.appendingPathComponent("The VERONA Break edit.wav")
            let fileManager = FileManager.default

            do {
                try fileManager.copyItem(at: file1URL!, to: fullDest1URL)
            } catch {
                print(error)
            }
            
            do {
                try fileManager.copyItem(at: file2URL!, to: fullDest2URL)
            } catch {
                print(error)
            }
        }

        return true
    }

    func applicationWillResignActive(_ application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    }

    func applicationWillEnterForeground(_ application: UIApplication) {
        // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    }

    func applicationWillTerminate(_ application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    }


}

