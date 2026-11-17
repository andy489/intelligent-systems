import java.lang.reflect.Method;

public class TimingUtil {

    public static void run(Object target, String methodName) {
        try {
            Method method = target.getClass().getDeclaredMethod(methodName);
            Timed timed = method.getAnnotation(Timed.class);
            String label = (timed != null && !timed.label().isEmpty()) ? timed.label() : methodName;

            long start = System.nanoTime();
            method.invoke(target);
            long elapsedMs = (System.nanoTime() - start) / 1_000_000;

            long s   = elapsedMs / 1000; long ms = elapsedMs % 1000;
            long min = s / 60;           s  %= 60;
            long h   = min / 60;         min %= 60;
            System.out.printf("[%s] %02d:%02d:%02d.%03d%n", label, h, min, s, ms);
        } catch (Exception e) {
            throw new RuntimeException("TimingUtil: failed to invoke " + methodName, e);
        }
    }
}
